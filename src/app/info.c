
virtual LPSTR getName() {
  LPSTR str = (LPSTR)HeapAlloc(GetProcessHeap(), 0, 15);
  strcpy(str, "DerivedClass_1");
  return str;
}

//inside main . . .

LPSTR str = BasePtr->getName();
printf("Name: %s\n", str);
HeapFree(GetProcessHeap(), 0, (void *)str);
