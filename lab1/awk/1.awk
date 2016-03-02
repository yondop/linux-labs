{
  if (match(FILENAME, subject"_"theme".*") != 0) {
    printf("%s ", $1);
  }
}

END {
  printf("были на контрольной %s\n", FILENAME);
}
