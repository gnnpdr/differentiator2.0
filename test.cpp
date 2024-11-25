char* my_strdup(const char *s)
{
    int i = 0;
    while(s[i] != '\0')
        i++;
    
    char* string = (char*)calloc(i + 1, sizeof(char));
    strcpy(string, s);
    return string;
}