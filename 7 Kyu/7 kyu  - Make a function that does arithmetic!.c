double arithmetic(double a, double b, char* operator) {

   switch (operator[0]) {
       case 'a':return a+b;break;
       case 's':return a-b;break;
       case 'm':return a*b;break;
       case 'd':return (b != 0 ) ? a/b : 0;break;
       default :return 0;break;
       }
}
