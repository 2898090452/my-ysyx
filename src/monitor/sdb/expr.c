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
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
enum {
  TK_NOTYPE = 256, TK_EQ,
  NUM,
  LPAREN=40,   //左括号
  RPAREN=41,   //右括号
};
int check_parentheses(int p,int q,int len,char *e); //判断表达式中的括号匹配
void check_enclosed(char *e,int len);   //判断整个表达式是否被括号包围
int check_inside(char *e,int len);
int check(int p,int q,char* e,int len);
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
struct stack
{
  char data;
  struct stack* next;
};
//判断栈是否为空
bool is_empty(struct stack* top)
{
  return (top==NULL);
}
//入栈
void push(struct stack **top,char data)
{
  struct stack* new=(struct stack*)malloc(sizeof(struct stack));
  new->data=data;
  new->next=*top;
  *top=new;
}
//出栈
char pop(struct stack** top)
{
  if(is_empty(*top)){
    return 0;
  }
  char data=(*top)->data;
  struct stack* temp=*top;
  *top=(*top)->next;
  free(temp);
  return data;
}
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

static bool make_token(char *e) {
  int pos1=0;
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
  /*check_parentheses(pos1,nr_token-1,nr_token,e);
  check_inside(e,nr_token);*/
  check(pos1,nr_token-1,e,position);
  return true;
}


int check(int p,int q,char* e,int len)
{
  //struct stack* top_z=NULL;
 // struct stack* top_y=NULL;

  int count_z=0;
  int count_y=0;
  int z,y;
  //char *str=e;
  for(int i=0;i<len;i++)
  {
    //char ch=e[i];
    if(e[i]==40)
    {
      zkh[count_z].type=40;
      zkh[count_z].pos=i;
      zkh[count_z].num=count_z;
      count_z++;
    }
    else if(e[i]==41)
    {
      ykh[count_y].type=41;
      ykh[count_y].pos=i;
      ykh[count_y].num=count_y;
      count_y++;
    }
  }
  if(count_y==count_z)
  {printf("括号数量匹配\n");}
  else if(count_y!=count_z)
  {printf("括号数量不匹配\n");}
  int temp=0;
  for(temp=0;temp<count_z;temp++)
  {
    printf("%d ",zkh[temp].pos);
  }
  printf("\n");
  for(temp=0;temp<count_y;temp++)
  {
    printf("%d ",ykh[temp].pos);
  }
  printf("\n");
  if(count_y==count_z)
  {
    int count=count_y;
    int array[count];
    int arr=0;
    int max=array[0];
    int max_dex;
    int counts=count+count;
    int match[counts];
    int match_count=0;
    for(int j=0;j<count;j++)
    {
      z=ykh[j].pos;
      for(int i=0;i<count_z;i++)
      {
        if(zkh[i].pos<ykh[j].pos)
        {
          array[arr]=zkh[i].pos;
          arr++;
        }
      }
      for(int i=0;i<arr;i++)
      { if(array[i]>=max)
        { max=array[i];
          max_dex=i;}
      }
      match[match_count]=ykh[j].pos;
      match[match_count]=max;
      match_count=match_count+2;


      for(int i=0;i<count_y;i++)//将右括号数组内的的第一个元素删去
      {
          ykh[i]=ykh[i+1];
      }
      count_y--;

    }
  }



  return 0;
}






bool is_matching(char left,char right){
  if(left==40&&right==41)
  {
    return true;
  }
  else
  return false;
}
int check_parentheses(int p,int q,int len,char *e)   //检查括号匹配函数
{
  //LPAREN=40,   //左括号
  //RPAREN=41,   //右括号
  //  int count=0;
  struct stack* top=NULL;
  for(int i=0;i<len;i++)
  {
    char str=e[i];
    if(str==40)
    push(&top,str);
      else if(str==41)
      {
        if(is_empty(top))
        {
          printf("括号不匹配\n");
          return -1;
        }
        char left=pop(&top);
        if(!is_matching(left,str))
        {
          printf("括号不匹配\n");
          return -1;
        }
      }
  }
  if(!is_empty(top))
    printf("括号不匹配\n");
  else
  printf("括号匹配\n");
  check_enclosed(e,len);
  return 0;



  /*int count_z=0;  //左括号计数器
  int count_y=0;  //右括号计数器
  int p1=p; //头
  int q1=q; //尾
  int count_zs=0;
  int count_ys=0;
  if(p1>=q1)
  { printf("表达式错误\n");
    return -1;  }
  //判断表达式是否被括号包围
  if((tokens[p1].type==40)&&(tokens[q1].type==41))
  {
    printf("表达式被括号包围\n");
  }
  else if((tokens[p1].type!=40)||(tokens[q1].type!=41))
  {
    printf("表达式没有被完整的括号包围\n");
  }
  //判断括号匹配
  while(p1<=q1)
  { 
    if(tokens[p1].type==40)
    {
      count_z++;
      if(tokens[p1+1].type==258)
        {
          count_zs++;
        }
      //p1++;
    }
    else if(tokens[p1].type==41)
    {
      count_y++;
      //p1++;
    }
    else{
      //p1++;
    }
    
  }
    if(count_z==count_y)
    {
      printf("括号数量匹配\n");
    }
    else if(count_z==count_y)
    {
      printf("括号数量不匹配\n");
    }
  return 0;*/
}

void check_enclosed(char *e,int len)
{
  if(e[0]!=40||e[len-1]!=41)
  {
    printf("整个表达式没有被括号包围\n");
  }
  else{
    printf("整个表达式被括号包围\n");
  }
}
//检查表达式内部的括号匹配
int check_inside(char *e,int len)
{
  struct stack* top=NULL;
  //struct stack* node=(struct stack*)malloc(sizeof(struct stack));//创建一个新节点
  for(int i=1;i<len-1;i++)
  {
    char str=e[i];
    if(e[i]==40)
    {   push(&top,str); 
        //node=top->next; //如果是左括号就把左括号后的数字传进去
        //push(&node,str);
    }
    else if(e[i]==41)
    {
      if(is_empty(top))
      {
        printf("内部括号不匹配\n");
        return -1;
      }
      char left=pop(&top);
      if(!is_matching(left,str))
        {
          printf("内部括号不匹配\n");
          return -1;
        }
      }
  }
  if(!is_empty(top))
    printf("内部括号不匹配\n");
  else
  printf("内部括号匹配\n");
  return 0;

    }


word_t expr(char *e, bool *success)
{
  if (!make_token(e)) 
  { 
    *success = false;
    return 0;
  }
  int j;
  //int temp=0;
  printf("开始打印token数组\n");
  for(j=0;j<nr_token;j++)
  {
    if(tokens[j].type==258)
    {printf("%d",tokens[j].num_type);}
    /*else if(tokens[j].type==40||tokens[j].type==41)
    {
      printf("%c的序号是%d\n",tokens[j].type,j);
    }*/
    else
    {printf("%c",tokens[j].type);}  
  }
  printf("\n");
  printf("打印完毕token数组\n");
  return 0;
}
