function prt(name,type)
{
	printf("\t .data\n");
	printf("\t .long $LC%d\n",FNR);
	printf("\t .long %s\n",name);
	s=1;
if(length(type)>19)
{
	type=substr(type,1,19);
	s=0;
}
	printf("\t .ascii	\"%s\\0\"\n",type);
	if(s)
	{
	printf("\t .space %i\n",20-length(type)-1);
	}
	printf("\t .section .rodata\n");
	printf("$LC%d:\n",FNR);
	printf("\t .ascii	\"%s\\0\"\n",name);
	
}
BEGIN {
			FS="[ \r\n]"
    		printf("gcc2_compiled.:\n");
			printf("__gnu_compiled_c:\n");
			printf("\t.globl	sym_tbl\n");
			printf("\t.globl	sym_tbl_size\n");
			printf("\t.data\n");
			printf("\t.align	2\n");
			printf("sym_tbl:\n");
			count=0
}
/./		{
			prt($1,$2);
			count++;
		}
END 		{

	      if(cpu=="I80386")
	      {
	         printf("\t .section .rodata\n");
	      } else 
	      {
	         printf("\t .section .sdata\n");
	      }
			printf("sym_tbl_size:\n");
			printf("\t.long %i\n",count);
}
