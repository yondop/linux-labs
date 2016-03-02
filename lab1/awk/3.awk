{
  if (FNR==1) printf("%s\n", FILENAME);
  if ($2>2) printf("%s\n", $0);
}
