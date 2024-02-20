/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/
#include <isa.h>
#include <regex.h>
#include <ctype.h>
enum {
  TK_NOTYPE = 256, TK_EQ,
  NUM,
  LPAREN=40,   //左括号
  RPAREN=41,   //右括号
};

int check(char* e,int len);
int findmax(int arr[],int size);

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},
  {"\\*", '*'},
  {"\\/", '/'},
  {"\\(",LPAREN},     //匹配左括号
  {"\\)",RPAREN},     //匹配右括号
  {"[0-9]",NUM},     //匹配0次或多次数字

  {"==", TK_EQ},        // equal
};

#define NR_REGEX ARRLEN(rules)  //NR_REGEX的值就等于rules数组中元素的个数

static regex_t re[NR_REGEX] = {};

void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    /* 第一个参数用于存储译好的正则表达式；
    第二个参数指向要编译的正则表达式的字符串；
    第三个参数表示编译正则表达式时使用的标志 */
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}
      
typedef struct token {
  int type;
  char str[32];
  int num_type;
} Token;
typedef struct kuohao{
  int pos;
  int type;
  int num;
}KH;

static KH zkh[50]={};
static KH ykh[50]={};
static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

int tokenlength(char *e)  //数组长度函数
{
  //int i;
  int position = 0;
  while(e[position]!='\0')
  {
    position++;
  }
  return position;
}
static bool make_token(char *e) {
  //int pos1=0;
  int position = 0;
  int i;
  int tip=0;
  //int lens=strlen(e);
  regmatch_t pmatch;
  nr_token = 0;
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) 
      { char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
       /* Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);*/
        position += substr_len;
  
        switch (rules[i].token_type) {
          case'+':
            tokens[nr_token].type='+';
            nr_token++;
            break;
          case'-':
            tokens[nr_token].type='-';
            nr_token++;
            break;
          case'*':
            tokens[nr_token].type='*';
            nr_token++;
            break;
          case'/':
            tokens[nr_token].type='/';
            nr_token++;
            break;
          case LPAREN:
            tokens[nr_token].type=LPAREN;
            nr_token++;
            break;
          case RPAREN:
            tokens[nr_token].type=RPAREN;
            nr_token++;
            break;
          case NUM :
            switch (e[position-1])
            {
              case '0':
              tokens[nr_token].type= NUM;
              tokens[nr_token].num_type=0;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '1':
              tokens[nr_token].type= NUM;
              tokens[nr_token].num_type=1;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '2':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=2;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '3':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=3;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '4':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=4;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '5':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=5;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '6':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=6;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '7':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=7;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '8':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=8;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              case '9':
              tokens[nr_token].type= NUM ;
              tokens[nr_token].num_type=9;
              tokens[nr_token].str[tip++]=substr_start[0];
              nr_token++;
              break;

              default:printf("here is default\n");
              break;
            }
           //printf("出switch\n");

          case TK_NOTYPE://空格不要
            break;
          default: TODO();
        }
        break;
      }
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  printf("nr_token=%d,positon=%d\n",nr_token,position);
  int lengthtk;
  lengthtk=tokenlength(e);  //测试数组长度函数
  printf("数组长度为%d\n",lengthtk);
  printf("数组长度为%d\n",position);
  check(e,position);
  return true;
}
int findmax(int arr[],int size) //寻找数组最大值
{
  int max=arr[0];
  int max_dex=0;
  for(int i=0;i<size;i++)
  {
    if(arr[i]>max)
    {
      max=arr[i];
      max_dex=i;
    }  
  }
  return max_dex;
}
int check(char* e,int len)  //括号匹配函数
{
  int count_z=0;
  int count_y=0;
  for(int i=0;i<len;i++)
  {
    if(e[i]==40)
    {
      //printf("这是左括号\n");
      zkh[count_z].type=40;
      zkh[count_z].pos=i;
      zkh[count_z].num=0;
      count_z++;
    }
    else if(e[i]==41)
    {
      //printf("这是右括号\n");
      ykh[count_y].type=41;
      ykh[count_y].pos=i;
      ykh[count_y].num=0;
      count_y++;
    }
  }
  if(count_y==count_z)
  {
    printf("括号数量匹配\n");
    int y_j=0;
    int z_i=0;
    int number=1;
    int arr[50]={};
    int index;
    int sz=0;
    int flag=0;
    for(y_j=0;y_j<count_y;y_j++)
    {
      z_i=0;
      ykh[y_j].num=number;
      while(z_i<count_z&&(zkh[z_i].pos<ykh[y_j].pos))
      {
        if(zkh[z_i].num==0)
        {
        //printf("%d ",arr[z_i]);
        arr[z_i]=zkh[z_i].pos;
        flag++;
       //printf("arr=%d ",arr[z_i]);
        z_i++;
        }
        else{
          z_i++;
        }
      }
      //printf("\n");
      //printf("flag=%d\n",flag);
      if(flag!=0){
        index=findmax(arr,50);//找到最大值的坐标
        //printf("最大值坐标%d\n",index);
        zkh[index].num=number;
        //printf("num=%d pos=%d\n",zkh[index].num,zkh[index].pos);
        for(sz=0;sz<50;sz++)
        {
          arr[sz]=0;
        }
        number++;
      }
      else if(flag==0)
      {
        printf("括号不匹配\n");
        return -1;
      }
      flag=0;
    }
  }
  else if(count_y!=count_z)
  {
    printf("括号数量不匹配\n");
    return -1;
  }
  
  int num_arr[50];
  int num_i=0;
  int num_dex;
  for(num_i=0;num_i<count_z;num_i++)
  {
    num_arr[num_i]=zkh[num_i].num;
  }
  num_dex=findmax(num_arr,50);
  if(num_dex==0)
  {
    printf("表达式被括号包围\n");
  }
  else{
    printf("整个表达式没有被括号包围\n");
  }
  int z_a=0;
  //打印括号位置和对应的num值
    for(z_a=0;z_a<count_z;z_a++)
    {
      printf("左位置=%d\t",zkh[z_a].pos);
    }
    printf("\n");
    for(z_a=0;z_a<count_z;z_a++)
    {
      printf("左数字=%d\t",zkh[z_a].num);
    }
    printf("\n");
    for(z_a=0;z_a<count_z;z_a++)
    {
      printf("右位置=%d\t",ykh[z_a].pos);
    }
    printf("\n");
    for(z_a=0;z_a<count_z;z_a++)
    {
      printf("右数字=%d\t",ykh[z_a].num);
    }
    printf("\n");
    int temp=0;
    for(temp=0;temp<count_z;temp++)
    {
      printf("左位置：%d ",zkh[temp].pos);
    }
    printf("\n");
    for(temp=0;temp<count_y;temp++)
    {
      printf("右位置：%d ",ykh[temp].pos);
    }
    printf("\n");
  printf("\n");
  return 0;
}

int eval(char *e) //表达式求值函数
{
  int tklen;
  tklen=tokenlength(e);
  printf("tokenlength=%d\n",tklen);
  if(tklen<1)
  {   return -1;  }
  else if(tklen==1)
  {
    if (isdigit(e[0]))  //检测字符串的第一个字符是不是数字
   {
        int num = atoi(e);
        printf("表达式的结果是数字: %d\n", num);
    } else 
    {
        printf("表达式错误\n");
    }
  }
  return 0;
  //else if(10)
}

word_t expr(char *e, bool *success)
{
  if (!make_token(e)) 
  { 
    *success = false;
    return 0;
  }
  /*int j;
  printf("开始打印token数组\n");
  for(j=0;j<nr_token;j++)
  {z.type==258)
    {printf("%d",tokens[j].num_type);}
    else
    {printf("%c",tokens[j].type);}  
  }
  printf("\n");
  printf("打印完毕token数组\n");*/
  eval(e);
  return 0;
}
