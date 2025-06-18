/OSBASE:=/{ printf "OSBASE\t:= %s\n", ocbase > dn "/makefile-1"; next  }
/OSBASE :=/{ printf "OSBASE\t:= %s\n", ocbase > dn "/makefile-1"; next  }
/OSBASE\t:=/{ printf "OSBASE\t:= %s\n", ocbase > dn "/makefile-1"; next  }
/OSBASE\t\t:=/{ printf "OSBASE\t\t:= %s\n", ocbase > dn "/makefile-1"; next  }
/pref:=/{ printf "pref\t:= %s\n", pref > dn "/makefile-1"; next  }
/pref :=/{ printf "pref\t:= %s\n", pref > dn "/makefile-1"; next  }
/pref\t:=/{ printf "pref\t:= %s\n", pref > dn "/makefile-1"; next  }
/pref\t\t:=/{ printf "pref\t\t:= %s\n", pref > dn "/makefile-1"; next  }
/./ { printf "%s\n",$0  > dn "/makefile-1" }
