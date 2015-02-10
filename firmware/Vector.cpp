// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Daniel Hay

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector() : count{0}, reserved{0}
{
    elements = NULL;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs) : count{rhs.count}, reserved{rhs.count}
{
    if (reserved >= 0 && rhs.elements != NULL)
    {
        elements = (T*) malloc(count * sizeof(T));
        for (int i = 0; i < count; ++i)
        {
            new (elements+i) T{rhs.elements[i]};
        }
    }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
   /* Vector<T> temp{rhs};
    std::swap(count, temp.count);
    std::swap(reserved, temp.reserved);
    std::swap(elements, temp.elements);
    return *this;*/
    if (this != &rhs)
    {
       for (int i = 0; i < count; ++i)
       {
           elements[i].~T();
       } 
       free(elements);
       elements = (T*)malloc(rhs.count*sizeof(T));
       count = rhs.count;
       reserved = rhs.reserved;
       for (int i = 0; i < count; ++i)
       {
           new (elements + i) T{rhs.elements[i]};
       }

    }
    return *this;
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved) : count{0}, reserved{nReserved}
{ 
    elements = (T*) malloc(nReserved*sizeof(T));
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t) : count{n}, reserved{n}
{
    elements = (T*)malloc(reserved*sizeof(T));
    for (int i = 0; i < n; ++i)
    {
        new (elements + i) T{t};
    }
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{
    if (reserved < n)
    {
        reserved = n;
        T* tmp = (T*)malloc(reserved*sizeof(T));
        for (int i = 0; i < count; ++i)
        {
            new (tmp+i) T{elements[i]};
            elements[i].~T();
        }
        free (elements);
        elements = tmp;
        tmp = NULL;
    }
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
    ////cout<<"DESTRUCTOR"<<endl;
    if (reserved > 0)     
    {
        for (int i = 0; i < count; ++i)
        {
            elements[i].~T();
        }
        free(elements);
    }
    count=0;
    reserved = 0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
    //cout<<"PushBack()"<<endl;
    //cout<<"elements="<<elements<<endl;
    if (count >= reserved)
    {
        
        //reserved = reserved == 0? 1 : reserved*2;
        //cout<<"Allocating space: "<<reserved<<endl;
       // T* tmp = (T*)malloc((reserved)*sizeof(T));//RESIZE
       
        T* tmp = (T*)malloc((count+1)*sizeof(T));//RESIZE
        reserved = count+1;
        //cout<<"tmp="<<tmp;

       


        //cout<<"copying elements"<<endl;
        for (int i = 0; i < count; ++i)
        {
            
            //cout<<"copy element: "<<i<<endl;

            new (tmp+i) T{elements[i]};
            //cout<<"deleting element"<<endl;
            elements[i].~T();
        }
        //cout<<"freeing elements"<<endl;
        if (NULL != elements)
            free(elements);
        //cout<<"Freed elements"<<endl;
        elements = tmp;
        tmp = NULL;


    }
    //cout<<"assigning rhs"<<endl;
    //cout<<"count="<<count<<"; reserved="<<reserved<<endl;
    //cout<<"elements+count="<<elements+count<<endl;
    new (elements + count++) T{rhs};
   
  
    //cout<<"exiting PushBack"<<endl;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
 
    if (count >= reserved)
    {
        //reserved = reserved == 0? 1 : reserved*2;
        //cout<<"Reserved="<<reserved<<endl;
       // T* tmp = (T*)malloc((reserved)*sizeof(T)); //RESIZE
        
        T* tmp = (T*)malloc((count+1)*sizeof(T)); //RESIZE
        reserved = count+1;
        if (tmp == NULL); //cout<<"NULLLLLLL"<<endl;
        for (int i = 0; i < count; ++i)
        {
            new (tmp+i+1) T{elements[i]};
            elements[i].~T();
        }
        free(elements);
        elements = tmp;
        tmp = NULL;
        new (elements) T{rhs};
    } else {
        for (int i = count; i > 0; --i)
        {
            elements[i] = elements[i-1];
        }
        
        new (elements) T{rhs};
    }
    ++count;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
    if (count <= 0)
        return;
    elements[--count].~T();
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
    ////cout<<"PopFront()"<<endl;
    if (count <= 0) return;
    //elements[0].~T();
    /*for (int i = 0; i < count - 1; ++i)
    {
        elements[i] = elements[i+1];
    }*/
    ////cout<<"deleteing last"<<endl;
        
    for (int i = 0; i < count - 1; ++i)
    {
        elements[i].~T();
        new (elements+i) T{elements[i+1]};
    }
    elements[--count].~T();
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
   
    return elements[0];
  
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
    return elements[count-1];

}

template <typename T>
T& Vector<T>::operator[](size_t i) const
{
    return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
    return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
    return count == 0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
    //cout<<"Clear"<<endl;
    for (int i = 0; i < count; ++i)
    {
        elements[i].~T();
    }
    count = 0;
    //cout<<"EXITING CLEAR"<<endl;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
    return VectorIterator<T>(elements + count); //END??
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
    VectorIterator<T> curr{it};
    VectorIterator<T> last{End().current-1};
    
    while (curr < last)
    {
        curr.current->~T();
        new ((curr).current) T{*(curr.current+1)};
        ++curr;
    }
    curr.current->~T();
    --count;
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
    int ind = it.current - elements;
    if (count >= reserved)
    {
       T* tmp = (T*)malloc(++reserved*sizeof(T));
      for (int i = 0; i < ind; ++i)
      {
          new (tmp+i) T{elements[i]};
          elements[i].~T();
      }
      new (tmp+ind) T{rhs};
      for (int i = ind; i < count; ++i)
      {
          new (tmp+i+1) T{elements[i]};
          elements[i].~T();
      }
      free(elements);
      elements = tmp;
      tmp = NULL;
    } else {
        for (int i = count; i > ind; --i)
        {
            new (elements + i) T{elements[i-1]};
            elements[i-1].~T();
        }
        new (elements+ind) T{rhs};
    }
    ++count;
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c) : current{c}
{
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs) : current{rhs.current}
{
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
    return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
    return VectorIterator<T>(++current);
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
    return VectorIterator<T>(current++);
}

template <typename T>
bool VectorIterator<T>::operator<(const VectorIterator& rhs) const
{
    return current < rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
    return current != rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
    return current == rhs.current;
}




