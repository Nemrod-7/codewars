const char leet1[26]={'@','8','(','D','3','F','6','#','!','J','K','1','M'
			      ,'N','0','P','Q','R','$','7','U','V','W','X','Y','2'};



char* toLeetSpeak (char *speak) {
	char *link = speak;

	while (*link != '\0') {
    if (*link >= 'A' && *link <= 'Z')
		*link = leet1[*link - 'A'];
		link++;
	}

return speak;
}
