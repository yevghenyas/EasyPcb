#ifndef SMARTPTR_H
#define SMARTPTR_H

#include<utility>
#include <new>

struct counter_block
{
   int count{0};
   bool bOneBlock{false};
};

template <class C>
class ptr_with_counter : public C
{
public:
   counter_block block;
   template<typename...Args>
   ptr_with_counter(Args&&...args):C(std::forward<Args>(args)...)
   {

   }
};






template <class C>
class SmartPtr
{
   C *p{nullptr};
   counter_block *pInfo{ nullptr };
   void releasePtr()
   {
      if (p)
      {
         --pInfo->count;
         if (pInfo->count == 0)
         {
            if (pInfo->bOneBlock)
            {
               delete p;
            }
            else
            {
               delete p;
               delete pInfo;
            }
         }
         p = nullptr;
         pInfo = nullptr;
      }
   }
   void assignPtr(const SmartPtr<C>& ptr)
   {
      ++ptr.pInfo->count;
      p = ptr.p;
      pInfo = ptr.pInfo;
   }
   void setPtrAndBlock(C *pC, counter_block *pBlock)
   {
      p = pC;
      pInfo = pBlock;
      pInfo->bOneBlock = true;
      if (p)
         ++pInfo->count;
   }
   void allocate(C *pp)
   {
      try
      {
         pInfo = new counter_block;
      }
      catch (std::bad_alloc&)
      {
         delete pp;
         throw;
      }
      pInfo->count = 1;
      pInfo->bOneBlock = false;
      p = pp;
   }
public:
   void setPtrAndBlockCrud(C *pC, counter_block *pBlock)
   {
      p = pC;
      pInfo = pBlock;
      if (p)
         ++pInfo->count;
   }
   SmartPtr() :p(nullptr), pInfo(nullptr)
   {
   }
   explicit SmartPtr(C* pp)
   {
      allocate(pp);
   }
   C* get()
   {
      return p;
   }
   SmartPtr(const SmartPtr<C>& ptr)
   {
      if (ptr.p)
      {
         assignPtr(ptr);
      }
   }
   void reset(C *pp)
   {
      releasePtr();
      if(pp)
         allocate(pp);
   }
   SmartPtr<C>& operator=(const SmartPtr<C>& ptr)
   {
      if (ptr.p)
      {
         releasePtr();
         assignPtr(ptr);
      }
   }
/*
   SmartPtr<C>&& operator=(SmartPtr<C>& ptr)
   {
      if(ptr.p)
        releasePtr();
   }
*/
   C* operator->()
   {
      return get();
   }

   template<class Base>
   operator SmartPtr<Base>()
   {
     SmartPtr<Base> ptr;
     ptr.setPtrAndBlockCrud(this->p,this->pInfo);
     return ptr;
   }
   ~SmartPtr()
   {
      releasePtr();
   }
   template <typename T,typename...Args>
   static SmartPtr<C> make_smartptr(Args&&...args)
   {
      auto p = new ptr_with_counter<T>(std::forward<Args>(args)...);
      counter_block *pp = &p->block;
      T *pT = p;
      SmartPtr<C> ptr;
      ptr.setPtrAndBlock(pT,pp);
      return ptr;
   }
};




#endif // SMARTPTR_H
