#pragma once
#include "TextNode.h"
#include <fstream>
#include <string>
class TextIter;

class Text {
protected:
  TextNode* root;
public:
  Text(char* s = 0);
  ~Text();

  TextIter GetRoot();
  TextIter Find(char c);
  TextIter Find(char* c);
  TextIter FindWord(char* c);


  void FileSave();
  void FileLoad(string tmp);

  void Insert(char* c, TextIter i);
  void DataInsert(char* c, TextIter i);
  void Delete(int count, TextIter i);
  char* Copy(int count, TextIter i);

  friend std::ostream& operator<<(std::ostream& o, Text& t);
};

class TextIter {
protected:
  Text& text;
  TextNode* node;
  TStack<TextNode*> st;
public:
  TextIter(Text& _text, TextNode* _node, TStack<TextNode*> _st);
  TextIter(const TextIter& t);

  bool GoNext(); 
  bool GoNextChar();
  bool IsEnd();

  void ResetToString();
  void ResetToWord();

  TextNode* Get();
};


TextIter::TextIter(Text& _text, TextNode* _node, TStack<TextNode*> _st) : text(_text) {
  node = _node;
  st = _st;
}

TextIter::TextIter(const TextIter& t) : text(t.text)
{
  node = t.node;
  st = t.st;
}

bool TextIter::GoNext()
{
  if (node->GetLevel() == 3)
  {
    if (node->GetNext() != 0) 
    {
      node = node->GetNext();
    }
    else
    {
      node = st.Get();

      if (node->GetNext() != 0) 
      {
        node = node->GetNext();
      }
      else
      {
        node = st.Get();
        if (node->GetNext() != 0) 
        {
          node = node->GetNext();
        }
        else
        {
          node = 0;
          return false;
        }
      }
    }
  }
  else
  {
    st.Push(node);
    node = node->GetDown();
  }
  return true;
}

bool TextIter::GoNextChar()
{
  if (node->GetLevel() == 1) 
  {
    st.Push(node);
    node = node->GetDown();
    st.Push(node);
    node = node->GetDown();
    return true;
  }

  if (node->GetLevel() == 2) 
  {
    st.Push(node);
    node = node->GetDown();
    return true;
  }

  if (node->GetLevel() == 3 && node->GetNext() != nullptr) 
  {
    node = node->GetNext();
    return true;
  }

  if (node->GetLevel() == 3 && node->GetNext() == nullptr) 
  {
    if (st.IsEmpty()) 
    {
      throw -1;
    }

    TextNode* temp = st.Get();

    if (temp->GetNext() != nullptr)
    {
      st.Push(temp->GetNext());
      node = temp->GetNext()->GetDown();
      return true;
    }

    return false;
  }
  return false;
}

bool TextIter::IsEnd()
{
  if (node == nullptr) 
  {
    return true;
  }

  if (node->GetLevel() == 1 || node->GetLevel() == 2) 
  {
    return false;
  }

  if (node->GetLevel() == 3 && node->GetNext() != nullptr) 
  {
    return false;
  }

  if (node->GetLevel() == 3 && node->GetNext() == nullptr) 
  {
    if (st.IsEmpty()) 
    {
      throw - 1;
    }

    TextNode* prev = st.Get();
    if (prev->GetNext() != nullptr) 
    {
      return false;
    }

    return true;
  }

  return true;
}

void TextIter::ResetToString() {
  while (node->GetLevel() > 1) 
  {
    if (st.IsEmpty())
    {
      throw - 1;
    }

    node = st.Get();
  }
}

void TextIter::ResetToWord()
{
  if (node->GetLevel() == 1)
  {
    st.Push(node);
    node = node->GetDown();
  }
  else if (node->GetLevel() == 3)
  {
    node = st.Get();
  }
}

TextNode* TextIter::Get() 
{
  return node;
}

Text::Text(char* s) 
{
  root = new TextNode(s, 1);
}

Text::~Text()
{
  TextNode::FreeMem();
}

TextIter Text::GetRoot()
{
  TStack<TextNode*> A(100);
  TextIter B(*this, root, A);
  return B;
}

TextIter Text::Find(char c)
{
  TextIter iter = GetRoot();

  while (iter.GoNextChar())
  {
    if (iter.Get()->GetC() == c)
    {
      return iter;
    }
  }
  return iter;
}

TextIter Text::FindWord(char* c) 
{
  TStack<TextNode*> A(100);
  TextIter iter(*this, root, A);

  while (iter.GoNext()) 
  {
    if (iter.Get()->GetLevel() == 2)
    {
      TextIter resuls = iter;
      bool control = true;
      int i = 0;
      int n = strlen(c);

      while (iter.GoNext() && i < n)
      {
        if (iter.Get()->GetLevel() == 2)
        {
          control = false;
          break;
        }

        if (iter.Get()->GetC() != c[i])
        {
          control = false;
          break;
        }
        i++;
      }

      if (control)
      {
        return resuls;
      }
    }
  }
  return iter;
}

TextIter Text::Find(char* s)
{
  TStack<TextNode*> A(100);
  TextIter iter(*this, root, A);
  int n = strlen(s);

  while (iter.GoNextChar()) 
  {
    TextIter cur = iter;
    int i = 0;
    bool control = true;
    bool next = true;
    while (i < n) 
    {
      if (!next)
      {
        control = false;
        break;
      }
      if (s[i] != cur.Get()->GetC()) {
        control = false;
        break;
      }
      next = cur.GoNextChar();
      i++;
    }

    if (control)
    {
      return iter;
    }
  }
}

void Text::FileSave()
{
    ofstream outf("Text.txt");
    if (!outf.is_open())
    {
      throw "Error file";
    }
    else
    {
      outf << *this;
      outf.close();
    }
}

void Text::FileLoad(string tmp)
{
  string l;
  
  ifstream inf(tmp);
  ifstream inf_2(tmp);

  if (!inf.is_open())
  {
    throw - 1;
  }
  else
  {
    getline(inf, l);
    inf.close();

    int size = l.length(); 
    char* text = new char[size+1];
    text[size] = '\0';

    if (!inf_2.is_open()) 
    {
      throw - 1;
    }
    else
    {
      for (int i = 0; i < size; i++)
      {
        text[i] = inf_2.get();
      }

      inf_2.close();
    }
    root = new TextNode(text, 1); 

    if (text != NULL)
    {
      delete[] text;
    }
  }
}

void Text::Insert(char* c, TextIter i)
{
  TextNode* node = new TextNode(c, i.Get()->GetLevel());
  TextNode* next = i.Get()->GetNext();
  node->SetNext(next);
  i.Get()->SetNext(node);
}

inline void Text::DataInsert(char* k, TextIter i)
{  
  if (strlen(k) == 1) 
  {
    Insert(k, i);
  }
  else 
  {
    int level = 2;
    for (int j = 0; j < strlen(k); j++)
    {
      if (k[j] == ' ') 
      {
        level = 1;
        break;
      }
    }

    TextIter glb = i;
    TextNode* next = i.Get()->GetNext();

    if (next->GetC() == ' ' || next->GetC() == '.')
    {
      i.GoNextChar();
      int size = strlen(k); 
      char* z = new char[strlen(k) + 1]; 
      z[strlen(k) + 1] = '\0';

      for (int j = 0; j < strlen(k); j++) 
      {
        z[j] = k[j];
      }
      z[strlen(k)] = ' '; 

      TextNode* node = new TextNode(z, level);

      next->SetNext(node);
      i.Get()->SetNext(node);
    }
    else
    {
      int count = 0;
      char symbol = ' ';
      TextNode* count_node = i.Get()->GetNext();

     
      while ((count_node->GetC() != '.') && (count_node->GetC() != ' ') && (count_node->GetC() != '\n'))
      {
        count++;
        count_node = count_node->GetNext();
      }

      if (count_node->GetC() == '.')
      {
        symbol = '.';
      }

      if (count_node->GetC() == '\n')
      {
        symbol = '\n';
      }

     
      char* tmp = new char[strlen(k) + count + 3];
      tmp[strlen(k) + count + 3] = '\0';
      tmp[0] = ' ';

      int j = 1;
      int g = 0;
      
      for (j; j < strlen(k) + 1; j++, g++)
      {
        tmp[j] = k[g];
      }
      tmp[j] = ' ';

      TextNode* tmp_node = i.Get()->GetNext();
      for (j = j + 1; j < strlen(k) + count + 2; j++)
      {
        tmp[j] = tmp_node->GetC();
        tmp_node = tmp_node->GetNext();
      }

      tmp[strlen(k) + count + 2] = symbol;

      glb.GoNext();
      this->Delete(count, glb);

      TextNode* node = new TextNode(tmp, level); 
      next->SetNext(node);
      i.Get()->SetNext(node);
    }
  }
}

void Text::Delete(int count, TextIter i)
{
  if (count == 0) 
  {
    return;
  }

  TStack<TextNode*> stack[3];
  int symb_del = 0;
  int level = i.Get()->GetLevel();


  if (level < 3)
  {
    i.GoNextChar();
  }

  if (level >= 1 && level <= 3)
  {
    stack[level - 1].Push(i.Get()); 
    if (level == 3)
    {
      symb_del++;
    }
  }

  TextIter iter = i;
  while (iter.GoNext() && symb_del < count)
  {
    level = iter.Get()->GetLevel();

    if (level >= 1 && level <= 3)
    {
      stack[level - 1].Push(iter.Get());

      if (level == 3)
      {
        symb_del++;
      }
    }
  }

  if (symb_del < count)
    throw - 1;


  if (stack[0].IsEmpty() && stack[1].IsEmpty())
  {
    TextIter prev = i;
    prev.ResetToWord(); 
    while (prev.Get()->GetNext() != i.Get() && prev.Get()->GetDown() != i.Get())
    {
      prev.GoNext();
    }
    TextNode* last = stack[2].Get();
    if (prev.Get()->GetLevel() == 2)
    {
      prev.Get()->SetDown(last->GetNext());
    }
    else
    {
      prev.Get()->SetNext(last->GetNext());
    }
    delete last;
    return;
  }

  TextIter prev = i;
  prev.ResetToWord();

  while (prev.Get()->GetNext() != i.Get() && prev.Get()->GetDown() != i.Get())
  {
    prev.GoNext();
  }

  if (prev.Get()->GetNext() == i.Get())
  {
    prev.Get()->SetNext(nullptr);
  }

  else
  {
    prev.Get()->SetDown(nullptr);
  }

  if (stack[0].IsEmpty())
  {
    TextNode* last_word = stack[1].Get();
    TextNode* last_letter = stack[2].Get();
    TextIter cur = i;
    cur.ResetToWord();
    cur.Get()->SetNext(last_word);
    last_word->SetDown(last_letter->GetNext());
    delete last_letter;
    for (int i = 1; i < 3; i++)
    {
      while (!stack[i].IsEmpty())
      {
        TextNode* del = stack[i].Get();
        delete del;
      }
    }
  }
  else 
  {
    TextNode* last_string = stack[0].Get();
    TextNode* last_word = stack[1].Get();
    TextNode* last_letter = stack[2].Get();
    TextIter cur = i;
    cur.ResetToString();
    cur.Get()->SetNext(last_string);
    cur.ResetToWord();
    cur.Get()->SetNext(last_word);
    last_word->SetDown(last_letter->GetNext());
    delete last_letter;

    for (int i = 0; i < 3; i++)
    {
      while (!stack[i].IsEmpty())
      {
        TextNode* tmp = stack[i].Get();
        delete tmp;
      }
    }
  }
}

char* Text::Copy(int count, TextIter i)
{
  if (count < 0)
  {
    throw - 1;
  }
    
  char* result = new char[count + 1];
  result[count] = '\0';
  TextIter iter = i;

  while (iter.Get()->GetLevel() < 3)
  {
    iter.GoNext();
  }

  if (count > 0) 
  {
    result[0] = iter.Get()->GetC();
    int j = 1;
    
    while (iter.GoNextChar() && j < count)
    {
      result[j] = iter.Get()->GetC();
      j++;
    }
  }

  
  return result;
}

std::ostream& operator<<(std::ostream& out, Text& t)
{
  out << *(t.root);
  return out;
}
