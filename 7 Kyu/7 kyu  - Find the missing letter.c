char findMissingLetter(char array[], int arrayLength) {

  int i, start = array[0];

  for (i = 0; i < arrayLength; ++i)
      if (array[i] != (start + i))
        return (array[i] - 1);
}
