param([int]$TargetPid)

Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class Win32 {
	public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
	[DllImport("user32.dll")] public static extern bool EnumWindows(EnumWindowsProc cb, IntPtr l);
	[DllImport("user32.dll")] public static extern bool EnumChildWindows(IntPtr p, EnumWindowsProc cb, IntPtr l);
	[DllImport("user32.dll")] public static extern int GetClassName(IntPtr h, StringBuilder s, int max);
	[DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr h, out uint pid);
	[DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
	[DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr h, uint msg, IntPtr w, IntPtr l);
	[DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
	[DllImport("user32.dll")] public static extern bool SetCursorPos(int x, int y);
	[DllImport("user32.dll")] public static extern void mouse_event(uint f, uint x, uint y, uint d, IntPtr e);
	[DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
	[DllImport("user32.dll")] public static extern IntPtr GetAncestor(IntPtr h, uint flags);
	[DllImport("kernel32.dll")] public static extern IntPtr OpenProcess(uint a, bool inh, uint pid);
	[DllImport("kernel32.dll")] public static extern bool CloseHandle(IntPtr h);
	[DllImport("kernel32.dll")] public static extern IntPtr VirtualAllocEx(IntPtr p, IntPtr addr, uint size, uint type, uint prot);
	[DllImport("kernel32.dll")] public static extern bool VirtualFreeEx(IntPtr p, IntPtr addr, uint size, uint type);
	[DllImport("kernel32.dll")] public static extern bool ReadProcessMemory(IntPtr p, IntPtr addr, byte[] buf, uint size, out uint read);
	[DllImport("kernel32.dll")] public static extern bool WriteProcessMemory(IntPtr p, IntPtr addr, byte[] buf, uint size, out uint written);
	[StructLayout(LayoutKind.Sequential)] public struct RECT { public int left, top, right, bottom; }
	[StructLayout(LayoutKind.Sequential)] public struct POINT { public int x, y; }
}
"@

$targetPid = $TargetPid
$script:tree = [IntPtr]::Zero

$findTree = {
	param($h, $l)
	$sb = New-Object System.Text.StringBuilder 256
	[Win32]::GetClassName($h, $sb, 256) | Out-Null
	if ($sb.ToString() -eq "SysTreeView32") {
		[uint32]$wp = 0
		[Win32]::GetWindowThreadProcessId($h, [ref]$wp) | Out-Null
		if ($wp -eq $targetPid) { $script:tree = $h; return $false }
	}
	return $true
}

$enumTop = {
	param($h, $l)
	[uint32]$wp = 0
	[Win32]::GetWindowThreadProcessId($h, [ref]$wp) | Out-Null
	if ($wp -eq $targetPid) {
		[Win32]::EnumChildWindows($h, $findTree, [IntPtr]::Zero) | Out-Null
	}
	if ($script:tree -ne [IntPtr]::Zero) { return $false }
	return $true
}

[Win32]::EnumWindows($enumTop, [IntPtr]::Zero) | Out-Null

if ($script:tree -eq [IntPtr]::Zero) { Write-Output "TREE_NOT_FOUND"; exit 2 }
Write-Output ("TREE_HWND=" + $script:tree)

# Message constants
$TVM_GETNEXTITEM = 0x110A
$TVGN_ROOT = 0x0
$TVGN_CHILD = 0x4
$TVGN_FIRSTVISIBLE = 0x5
$TVGN_NEXTVISIBLE = 0x6
$TVM_GETITEMRECT = 0x1104

# Use the first VISIBLE item, then step to the next visible one so we click a
# real, on-screen tree node (root may be scrolled/partially clipped).
$first = [Win32]::SendMessage($script:tree, $TVM_GETNEXTITEM, [IntPtr]$TVGN_FIRSTVISIBLE, [IntPtr]::Zero)
if ($first -eq [IntPtr]::Zero) { Write-Output "NO_VISIBLE_ITEM"; exit 3 }
$next = [Win32]::SendMessage($script:tree, $TVM_GETNEXTITEM, [IntPtr]$TVGN_NEXTVISIBLE, $first)
$item = if ($next -ne [IntPtr]::Zero) { $next } else { $first }
Write-Output ("CLICK_ITEM=" + $item)

# Cross-process: write HTREEITEM into remote RECT buffer, call TVM_GETITEMRECT (text portion=TRUE)
$PROCESS_VM = 0x0008 -bor 0x0010 -bor 0x0020 -bor 0x0400
$hProc = [Win32]::OpenProcess($PROCESS_VM, $false, [uint32]$targetPid)
$remote = [Win32]::VirtualAllocEx($hProc, [IntPtr]::Zero, 16, 0x3000, 0x04)

# Bring the app's top-level window to the foreground so real mouse clicks land on it.
$top = [Win32]::GetAncestor($script:tree, 2) # GA_ROOT
[Win32]::SetForegroundWindow($top) | Out-Null
Start-Sleep -Milliseconds 300

function Get-ItemRect([IntPtr]$it) {
    $itemBytes = [BitConverter]::GetBytes([int64]$it)
    [uint32]$w = 0
    [Win32]::WriteProcessMemory($hProc, $remote, $itemBytes, 8, [ref]$w) | Out-Null
    [Win32]::SendMessage($script:tree, $TVM_GETITEMRECT, [IntPtr]1, $remote) | Out-Null
    $buf = New-Object byte[] 16
    [uint32]$r = 0
    [Win32]::ReadProcessMemory($hProc, $remote, $buf, 16, [ref]$r) | Out-Null
    return @(
        [BitConverter]::ToInt32($buf,0), [BitConverter]::ToInt32($buf,4),
        [BitConverter]::ToInt32($buf,8), [BitConverter]::ToInt32($buf,12)
    )
}

function Click-Item([IntPtr]$it) {
    $rc = Get-ItemRect $it
    $rl=$rc[0]; $rt=$rc[1]; $rr=$rc[2]; $rb=$rc[3]
    if ($rb -le 0 -or $rt -lt 0) { Write-Output ("SKIP_OFFSCREEN item=$it rect=$rl,$rt,$rr,$rb"); return }
    $cx = [int](($rl + $rr) / 2)
    $cy = [int](($rt + $rb) / 2)
    $pt = New-Object Win32+POINT; $pt.x = $cx; $pt.y = $cy
    [Win32]::ClientToScreen($script:tree, [ref]$pt) | Out-Null
    [Win32]::SetCursorPos($pt.x, $pt.y) | Out-Null
    Start-Sleep -Milliseconds 150
    [Win32]::mouse_event(0x0002, 0,0,0,[IntPtr]::Zero) # LEFTDOWN
    Start-Sleep -Milliseconds 60
    [Win32]::mouse_event(0x0004, 0,0,0,[IntPtr]::Zero) # LEFTUP
    Start-Sleep -Milliseconds 400
    Write-Output ("CLICKED item=$it screen=$($pt.x),$($pt.y)")
}

# Click several visible items in sequence to thoroughly exercise OnClick.
$cur = [Win32]::SendMessage($script:tree, $TVM_GETNEXTITEM, [IntPtr]$TVGN_FIRSTVISIBLE, [IntPtr]::Zero)
$count = 0
while ($cur -ne [IntPtr]::Zero -and $count -lt 8) {
    Click-Item $cur
    $count++
    $cur = [Win32]::SendMessage($script:tree, $TVM_GETNEXTITEM, [IntPtr]$TVGN_NEXTVISIBLE, $cur)
}

[Win32]::VirtualFreeEx($hProc, $remote, 0, 0x8000) | Out-Null
[Win32]::CloseHandle($hProc) | Out-Null
Start-Sleep -Milliseconds 500
Write-Output "CLICKS_SENT count=$count"
