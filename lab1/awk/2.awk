BEGIN {
  s = 0.0;
  count = 0;
  lastFile = "";
}

{
  if (FNR == 1 && count > 0) {
      printf("> %s: %f\n", lastFile, s / count);
      count = 0;
      s = 0;
  }

  s += $2;
  count++;
  lastFile = FILENAME;
}

END {
  printf("> %s: %f\n", FILENAME, s / count);
}
