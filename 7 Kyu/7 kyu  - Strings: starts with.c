int startsWith(const char *string, const char *prefix) {

  int ret = strncmp (string,prefix,strlen(prefix));

return (ret == 0) ? 1 : 0;
}
