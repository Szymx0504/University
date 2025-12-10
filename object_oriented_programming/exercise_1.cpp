#include <iostream>
#include <vector>

struct ListNode
{
   int value;
   ListNode *next;
   ListNode() : value(0), next(nullptr) {}
   ListNode(int v) : value(v), next(nullptr) {}
   ListNode(int v, ListNode *next) : value(v), next(next) {}
   ListNode(const std::vector<int> &values)
   {
      if (values.empty())
      {
         value = 0;
      }
      else
      {
         value = values[0];
      }
      next = nullptr;
      ListNode *it = this;
      for (size_t i = 1; i < values.size(); ++i)
      {
         it->next = new ListNode(values[i]);
         it = it->next;
      }
   }

   ~ListNode()
   {
      delete next;
   }
};

std::ostream &operator<<(std::ostream &os, ListNode *node)
{
   ListNode *n = node;
   while (n->next != nullptr)
   {
      os << n->value << " -> ";
      n = n->next;
   }

   if (n != nullptr)
   {
      os << n->value;
   }

   return os;
}

ListNode *merge(ListNode *list1, ListNode *list2)
{
   if (list1 == nullptr)
      return list2;
   if (list2 == nullptr)
      return list1;
   ListNode *head = nullptr;
   if (list1->value < list2->value)
   {
      head = list1;
      list1 = list1->next;
   }
   else
   {
      head = list2;
      list2 = list2->next;
   }
   ListNode *curPtr = head;
   while (list1 != nullptr && list2 != nullptr)
   {
      if (list1->value < list2->value)
      {
         curPtr->next = list1;
         list1 = list1->next;
      }
      else
      {
         curPtr->next = list2;
         list2 = list2->next;
      }
      curPtr = curPtr->next;
   }
   if (list1 != nullptr)
   {
      curPtr->next = list1;
   }
   if (list2 != nullptr)
   {
      curPtr->next = list2;
   }

   return head;
}

int main()
{
   int n1;
   std::cin >> n1;
   std::vector<int> vec1;
   for (int i = 0; i < n1; ++i)
   {
      int value;
      std::cin >> value;
      vec1.push_back(value);
   }

   int n2;
   std::cin >> n2;
   std::vector<int> vec2;
   for (int i = 0; i < n2; ++i)
   {
      int value;
      std::cin >> value;
      vec2.push_back(value);
   }

   ListNode *list1 = vec1.empty() ? nullptr : new ListNode(vec1);
   ListNode *list2 = vec2.empty() ? nullptr : new ListNode(vec2);

   ListNode *result = merge(list1, list2);

   if (result)
   {
      std::cout << result << std::endl;
   }
   else
   {
      std::cout << std::endl;
   }
   
   delete result;

   return 0;
}