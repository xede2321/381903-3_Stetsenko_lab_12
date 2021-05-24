#pragma once
#include <iostream>
#include "Stack.h"
#include <cstring>

class TextNode
{
protected:
  static TextNode* firstFree;
  static TextNode* textNodes;
  static TextNode* textNodes_tmp;

  TextNode* next; 
  TextNode* down;
  char c;
  int level; 

public:
  TextNode(int _l = 3, char _c = 0);
  TextNode(char* _c, int _l);
  TextNode(const TextNode& _node);
  ~TextNode();

  TextNode* GetNext(); 
  TextNode* GetDown();
  char GetC();
  int GetLevel();

  void SetNext(TextNode* _next);
  void SetDown(TextNode* _down);
  void SetC(char _c);
  void SetLevel(int _l);

  static void InitMem(int _size = 10); 
  static void FreeMem(); 
  
  void* operator new(size_t size);
  void operator delete(void* p);

  friend std::ostream& operator<<(std::ostream& o, TextNode& t);
};

TextNode* TextNode::firstFree = 0;
TextNode* TextNode::textNodes = 0;
TextNode* TextNode::textNodes_tmp = 0;

std::ostream& operator<<(std::ostream& o, TextNode& t)
{
  if (t.level == 3)
  {
    o << t.c;
    if (t.next != nullptr)
      o << *(t.next);
  }
  else
  {
    if (t.down != nullptr)
      o << *(t.down);

    if (t.next != nullptr)
      o << *(t.next);
  }

  return o;
}

TextNode::TextNode(int _l, char _c)
{
  next = nullptr;
  down = nullptr;
  c = _c;
  level = _l;
}

TextNode::TextNode(char* _c, int l)
{
  if (l < 1 || l > 3)
    throw - 1;

  if (l == 1) {
    next = nullptr;
    c = 0;
    level = l;
    int size = strlen(_c);

    if (size > 0)
    {
      int start = 0; 
      TextNode* wcur = new TextNode(2);
      this->SetDown(wcur);

      for (int i = 0; i < size; i++)
      {
        if (_c[i] == ' ' || i == size - 1) 
        {
          if (start != 0) 
          {
            wcur->SetNext(new TextNode(2)); 
            wcur = wcur->GetNext();
          }

          TextNode* lcur = new TextNode(3, _c[start]); 
          wcur->SetDown(lcur); 

          for (int j = start + 1; j <= i; j++) 
          {
      
            lcur->SetNext(new TextNode(3, _c[j]));
            lcur = lcur->GetNext();
          }

          lcur->SetNext(nullptr);
          start = i + 1;
        }
      }
      wcur->SetNext(nullptr);
    }
  }

  if (l == 2) {
    next = nullptr;
    c = 0;
    level = l;
    int size = strlen(_c);
    if (size > 0) 
    {
      TextNode* lcur = new TextNode(3, _c[0]);
      this->SetDown(lcur);

      for (int i = 1; i < size; i++) 
      {
        lcur->SetNext(new TextNode(3, _c[i]));
        lcur = lcur->GetNext();
      }
      lcur->SetNext(nullptr);
    }
  }

  if (l == 3) 
  {
    c = _c[0];
    level = l; 
    next = nullptr;
    down = nullptr;
  }
}

TextNode::TextNode(const TextNode& _node)
{
  next = nullptr;
  down = nullptr;
  c = _node.c;
  level = _node.level;
}

TextNode::~TextNode()
{
}

TextNode* TextNode::GetNext()
{
  return next;
}

TextNode* TextNode::GetDown()
{
  return down;
}

char TextNode::GetC()
{
  return c;
}

int TextNode::GetLevel()
{
  return level;
}

void TextNode::SetNext(TextNode* _next)
{
  next = _next;
}

void TextNode::SetDown(TextNode* _down)
{
  down = _down;
}

void TextNode::SetC(char _c)
{
  c = _c;
}

void TextNode::SetLevel(int _l)
{
  if (_l >= 1 && _l <= 3)
  {
    level = _l;
  }
  else
  {
    throw "Error";
  }
}

void TextNode::InitMem(int _size)
{
  if (_size <= 0)
    throw "Error";

  if ((firstFree == 0) && (textNodes == 0))
  {
    textNodes = new TextNode[_size];
    firstFree = textNodes;
  }
  else if ((firstFree != 0) && (textNodes == 0))
  {
    throw "Error";
  }
  else 
  {
    textNodes_tmp = new TextNode[_size];

    for (int i = 0; i < _size; i++)
    {
      textNodes_tmp[i] = (textNodes[i]);
    }

    textNodes = new TextNode[_size * 2]; 
    firstFree = textNodes;

    for (int i = 0; i < _size; i++)
    {
      textNodes[i] = textNodes_tmp[i];
    }

    delete[] textNodes_tmp; 
    textNodes_tmp = 0;
  }
}

void TextNode::FreeMem() 
{
  if (textNodes != 0)
  {
    delete[] textNodes;
  }
  textNodes = 0;
  firstFree = 0;
}

void* TextNode::operator new(size_t size)
{
  if ((firstFree == 0) && (textNodes == 0))
    InitMem();

  if ((firstFree == 0) && (textNodes != 0))
    InitMem();

  TextNode* node = firstFree;
  firstFree = firstFree->next;

  node->next = 0;

  return node;
}

void TextNode::operator delete(void* p)
{
  TextNode* node = (TextNode*)p;

  if ((firstFree == 0) && (textNodes == 0))
    throw "Error";

  node->next = nullptr;
  node->down = nullptr;
  node->c = 0;
  node->level = 3;

  node->next = firstFree;
  firstFree = node;
}