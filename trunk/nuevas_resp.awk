#BEGIN{FS="\n";RS=""}
#BEGIN{ a=substr(ARGV[3],3) }
{
 if(NR==1) {
   f=FILENAME
 }
 
if(f==FILENAME) {
      print >  "./file.back"
      #print >  a 
 }
 else {
   if($0=="") {
     f=FILENAME
     print"" > "./file.back"
     #print"" > a
   }
 }
}
