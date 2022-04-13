int stringToInt(char linea[],int ind){
  int i,k,cant,num;
  k = cant = num = 0;
  //printf("%s\n",linea);
  for(i=ind;i>=0;i--){
   k = linea[i] - '0';
   //printf("%d\n",k);
   k = k << cant++;
   num = num | k;
  }
  //printf("%d\n",num);
  return num;
}
