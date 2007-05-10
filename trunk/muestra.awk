#!/usr/bin/awk
BEGIN {
	FS=":";RS="\v";
}
{ nombre=substr(ARGV[1],3)
  if($3~nombre){
	  print $4,"...",$5
  }
	
}

