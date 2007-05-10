BEGIN{RS="";FS="\n"; }
{
  if(NR==1) {
     printf("%-17s","   Apellidos")
     ###
     ##
     cont=NF-1;
     for(i=1;i<=cont;i++)
       printf("%d.",i%10);
     print ""
     ###
     ##
  }
  n=split($1,A,":")
  nombre=A[3]
  NOMBRE[++t]=nombre
  aciertos=0
  for(i=2;i<=NF;i++)
  {
    n=split($i,A,":")
    if(A[2]=="b"){
      R[nombre]=R[nombre]"b."
      resp_cont[A[1]]++ 
      ac[nombre]++
      aciertos++
    }
    else {
      resp_m_cont[A[1]]++
      R[nombre]=R[nombre]"m."
   }
  }
  cont_t++
  printf("\n%-3d%-14s%s %d",++i1,substr(nombre,1,index(nombre," ")),R[nombre],
                                     ac[nombre])
}
END{
  printf("\n\nLos %d alumnos contestaron así:\n\n",cont_t)
  printf("%-7s","Preg.")
  for(i=1;i<=cont;i++) {
      printf("%-4d",i)
  }
  printf("\n%-7s","Bien")
  for(i=1;i<=cont;i++) {
      printf("%-4d",resp_cont[i])
  }
  printf("\n\n\nNum.  Apellidos y Nombres          Aciertos -de %d -\n\n",cont)
  for(i=1;i<=t;i++)
   printf("%-3d%-35s %d\n",i,NOMBRE[i],ac[NOMBRE[i]])
}
