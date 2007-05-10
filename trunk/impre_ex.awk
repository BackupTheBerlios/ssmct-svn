BEGIN{FS="\v"; RS="\f";es=0;primero=1
#ORS="\f"
}
{
  split($1,A,":");
  n=split(e,B,",");
  print "-x-x-x-x-x-x-x-x-x-x-x-x-"NR"-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-\n"
  print "-----------------------------------"
  print "|[b]"NR"[b]|.",A[3];
  print "-----------------------------------"
  for(i=1;i<=n;i++) {
     if(B[i]==NR)
       es=1
  }
  if(es) {
    print "ELIMINADO"
    es=0
  }
  else {
    print "-----------------------------------"  > ARGV[1]".lpt"
    print NR".",A[3] > ARGV[1]".lpt"
    print "-----------------------------------"  > ARGV[1]".lpt"
    for(i=1;i<=NF;i++) {
      print $i
      if(i!=1) {
        print $i"\n" >  ARGV[1]".lpt"
      }
      print "";print"--";print""
    }
    print "\f" >  ARGV[1]".lpt"
#    ORS="\f";
    if(!primero) {
      print "\f" >  ARGV[1]".new"
    }
    else {
     primero=0
    }
    print $0 >  ARGV[1]".new"
#    ORS="\n";
  }
}
