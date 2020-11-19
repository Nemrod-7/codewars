std::string encrypt(std::string source, int n) {

  if (n <= 0) return source;

  std::string even, odd, os;

  for (size_t i = 0; i < source.size(); i++) {
      if (i % 2)
          even += source[i];
      else
          odd += source[i];
  }
  os = even + odd;

    return  encrypt (os, n - 1);
}
std::string decrypt(std::string src, int n) {

  if (n <= 0) return src;

  std::string os;
  int o = 0, e = src.size() / 2;

  for (size_t i = 0; i < src.size(); i++)
      os += i % 2 ? src[o++] : src[e++];

  return  decrypt (os, n - 1);
}
