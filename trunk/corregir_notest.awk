BEGIN{FS=":"; primero=1;aciertos=0;cont=0;i1=0;
  #
printf("%-17s","   Apellidos")
}
{
if($0!=""&&primero) {
  n=index($0,":");
  resp[$1]=substr($0,n+1)
  cont++;
  #printf("%-2d ",cont%10);
  printf("%d.",cont%10);
  nom=1;
}
if($0==""&&!primero) {
  
  printf("\n%-3d%-14s%s %d",++i1,substr(nombre,1,index(nombre," ")),R[nombre],
                                     ac[nombre])
  NOMBRE[++t]=nombre
#,aciertos,cont);
  aciertos=0
  nom=1
}
if($0==""&&primero) {
  primero=0;
  print ""
}
if($0!=""&&!primero&&!nom) {
 n=split(resp[$1],A)
 if(n==0) {  # para el caso que hay NADA en Respuestas y nada en resp. alumno
  n=1
  A[1]=""
 }
 for(i=1;i<=n;i++) {      
   if(A[i]==$2){
     #printf("www:%swww%swww\n",A[i],$2);
     R[nombre]=R[nombre]"b."
     resp_cont[$1]++ 
     ac[nombre]++
     aciertos++
     break;
   }
 }
 if(i>n) {
   resp_m_cont[$1]++
   R[nombre]=R[nombre]"m."
 }
}
if($0!=""&&!primero&&nom) {
  nombre=$3
  nom=0
  aciertos=0
  cont_t++
}
}
END{
  #printf("\n%-25s   %s    %s / %s\n",nombre,R[nombre],aciertos,cont);
  #printf("\n%-14s%-2s  %s / %s\n",substr(nombre,1,index(nombre," ")),R[nombre],aciertos,cont);
  printf("\n%-3d%-14s%s %d\n\n",++i1,substr(nombre,1,index(nombre," ")),R[nombre],
                                   ac[nombre])
  NOMBRE[++t]=nombre
  printf("Los %d alumnos contestaron así:\n\n",cont_t)
  printf("%-7s","Preg.")
  for(i=1;i<=cont;i++) {
      #printf("%d: %d/%d\n",i,resp_cont[i],resp_cont[i]+resp_m_cont[i])
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
