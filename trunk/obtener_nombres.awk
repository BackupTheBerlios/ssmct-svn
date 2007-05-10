BEGIN{FS="\v"; RS="\f";
}
{
  split($1,A,":");
  print A[3];
}
