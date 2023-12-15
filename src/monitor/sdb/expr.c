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
int check_parentheses(int p,int q);
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
           // printf("出switch\n");

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
  check_parentheses(pos1,nr_token-1);
  return true;
}

int check_parentheses(int p,int q)   //检查括号匹配函数
{
  //LPAREN=40,   //左括号
  //RPAREN=41,   //右括号
  printf("666\n");
  int count=0;
  if(p>=q)
  { printf("表达式错误\n");
    return -1;  }
  //判断表达式是否被括号包围
  //printf("tokens[p].type=%d tokens[q].type=%d\n",tokens[p].type,tokens[q].type);
  //printf("左括号%c\n右括号%c\n",tokens[p].type,tokens[q].type );
  if((tokens[p].type==40)&&(tokens[q].type==41))
  {
    printf("表达式被括号包围\n");
  }
  else if((tokens[p].type!=40)||(tokens[q].type!=41))
  {
    printf("表达式没有被完整的括号包围\n");
  }
  //判断括号匹配
  while(p<=q)
  { 
    if(tokens[p].type==40)
    {
      count++;
      p++;
    }
    else if(tokens[p].type==41)
    {
      count--;
      p++;
    }
    else{
      p++;
    }
  }
    if(count==0)
    {
      printf("括号数量匹配\n");
    }
    else if(count!=0)
    {
      printf("括号数量不匹配\n");
    }
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
