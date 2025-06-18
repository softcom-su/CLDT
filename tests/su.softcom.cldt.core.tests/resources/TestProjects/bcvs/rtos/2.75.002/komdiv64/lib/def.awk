function prt(name,type)
{
	printf("%s %s\n",name,type);
}
BEGIN 		{
			FS="[\t{( *)},\"]"
		}
/./		{
			if(FILENAME=="out1") {
			   types[$4]=$8;
#			printf "%s~%s\n",$4,$8; 
			   next;
			} 
		}
/\.text/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"fi");
			  count++;
		          next;
		}
/\*COM\*/		{
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}
/\.data/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}
/\.sdata/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}
/\.bss/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}
/\.sbss/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}
/\.scommon/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}

/\.rodata/		{
                          if($2=="g")
			   if($NF in types) {
				prt($NF,types[$NF]); 
			   }  else prt($NF,"gi");
			  count++;
		          next;
		}
