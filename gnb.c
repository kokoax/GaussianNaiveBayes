#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NAME_BUF 256
#define AMOUNT    75
#define LENGTH     4
#define FILE_NAME "input/even.data"
#define TEST_FILE_NAME "input/odd.data"

typedef struct data {
  char   name[NAME_BUF];
  double num[LENGTH];
} DATA;

double average(const double *data);
double standard_deviation(const double *data, const double avg);

void gauss_naive_bayes(DATA *target);
void set_data(DATA data_set[AMOUNT], char data_names[AMOUNT][NAME_BUF]);

int is_answer(DATA *target){
  char ans_name[NAME_BUF];
  strcpy(ans_name, target->name);
  gauss_naive_bayes(target);
  printf("%s == %s\n", ans_name, target->name);
  if( !strcmp(ans_name, target->name) ){
    return 0;
  }
  return 1;
}

void test(void) {
  FILE *fp;
  int i,j;
  DATA targets[75];
  double match_num = 0;

  if( (fp = fopen(TEST_FILE_NAME, "r")) == NULL ){
    printf("Not found test file\n");
    exit(-1);
  }
  for( i = 0; i < 75; i++ ){
    for( j = 0; j < LENGTH; j++ ){
      fscanf(fp, "%lf,", &targets[i].num[j]);
    }
    fscanf(fp, "%s", targets[i].name);
    printf("Test %d Start\n", i);
    printf("Test Data: %lf %lf %lf %lf %s\n\n", targets[i].num[0], targets[i].num[1], targets[i].num[2], targets[i].num[3], targets[i].name);
    if( !is_answer(&targets[i]) ){
      match_num += 1;
    } else {
      printf("Missing!!\n");
    }
    printf("Test %d End\n\n", i);
  }
  printf("%.0lf / %d = %lf\n", match_num, 75, match_num/75);
  fclose(fp);
}

int main(void){
  test();
}

void add_name(char target[NAME_BUF], char names[AMOUNT][NAME_BUF]){
  int i = 0;
  while( names[i++][0] != '\0' ){
    if( !strcmp(names[i-1], target) ){
      return;
    }
  }
  // printf("%d\n", i-1);
  strcpy(names[i-1], target);
  return;
}

// data.datからデータを取得しdata_set変数に代入
void set_data(DATA data_set[AMOUNT], char names[AMOUNT][NAME_BUF]){
  FILE *fp;
  int i, j;
  double ans = 0;

  if( (fp = fopen(FILE_NAME, "r")) == NULL){
    printf("Not found %s\n", FILE_NAME);
    exit(-1);
  }

  for( i = 0; i < AMOUNT; i++ ){
    ans = 0;
    names[i][0] = '\0'; //namesの初期化
    for( j = 0; j < LENGTH; j++ ){
      fscanf(fp, "%lf,", &data_set[i].num[j]);
    }
    fscanf(fp, "%s", data_set[i].name);
    add_name(data_set[i].name, names);
  }

  fclose(fp);
}

double average(const double *data){
  int i = 0;
  double sum = 0.0;
  while(data[i++] != EOF){
    sum += data[i-1];
  }
  return sum/i;
}

double standard_deviation(const double *data, const double avg){
  int i = 0;
  double sum = 0.0;
  while(data[i++] != EOF){
    sum += pow(data[i]-avg, 2);
  }
  return sqrt(sum/i);
}

void gauss_naive_bayes(DATA *target){
  int i,j;
  int name_index;
  DATA data_set[AMOUNT];
  char data_names[AMOUNT][NAME_BUF];

  double data_tmp[AMOUNT+1];
  int data_tmp_i;
  double avg;
  double sd;
  double p[LENGTH];
  double ps[AMOUNT];
  int ps_i = 0;
  double tmp_p = 1.0;
  int max_i = 0;
  double max_p = 0.0;

  set_data(data_set, data_names);

  for( i = 0; i < AMOUNT; i++ ){
    if( data_names[i][0] == '\0' ){
      break;
    }
    // printf("%s\n", data_names[i]);
  }

  for( name_index = 0; name_index < AMOUNT; name_index++ ){
    if( data_names[name_index][0] == '\0' ) break;
    printf("group name: %s\n", data_names[name_index]);
    for( i = 0; i < LENGTH; i++ ){
      data_tmp_i = 0;
      for( j = 0; j < AMOUNT; j++ ){
        if( !strcmp(data_set[j].name, data_names[name_index]) ){
          data_tmp[data_tmp_i++] = data_set[j].num[i];
        }
      }
      data_tmp[data_tmp_i] = EOF;
      avg = average(data_tmp);
      sd   = standard_deviation(data_tmp, avg);
      printf("avg:%lf sd:%lf\n", avg, sd);
      p[i] = (1 / sqrt(2*M_PI*pow(sd,2))) * exp(-1 * pow(target->num[i]-avg,2) / (2*pow(sd,2)));
      printf("%lf\n", p[i]);
    }
    tmp_p = 1.0;
    for( i = 0; i < LENGTH; i++ ){
      tmp_p *= p[i];
    }
    ps[ps_i++] = tmp_p;
    ps[ps_i] = -1;
    printf("p = %lf\n", ps[ps_i-1]);
    printf("\n");
  }
  max_i = 0;
  max_p = 0.0;
  ps_i  = 0;
  while( ps[ps_i++] != -1 ){
    if( ps[ps_i-1] > max_p ){
      max_p = ps[ps_i-1];
      max_i = ps_i-1;
    }
  }
  strcpy(target->name, data_names[max_i]);
}

