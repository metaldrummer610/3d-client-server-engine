#ifndef MEMORY_MANAGEMENT_HH
#define MEMORY_MANAGEMENT_HH

#include <list>
#include <string>

#define AUTO_SIZE unsigned long size(){return sizeof(*this);}
#define AUTO_PTR(X) typedef CMMPointer<X> Ptr;

#define IMMHEADER(X) \
	AUTO_SIZE \
	AUTO_PTR(X)

class IMMObject
{
   private:
      static std::list<IMMObject *> liveObjects;
      static std::list<IMMObject *> deadObjects;
      long refCount;
   protected:
      IMMObject();
      virtual ~IMMObject();
   public:
      void AddRef();
      void Release();
      static void CollectGarbage();
      static void CollectRemainingObjects(bool bEmitWarnings=false);
      virtual unsigned long size()=0;
	  virtual std::string toString() = 0;
	
		template <typename T>
		static std::string toString(T* obj) {
			return std::string("IMMObject");
		}

};

template<class T>
class CMMPointer
{
protected:
   T* obj;
public:

   //Constructors - basic
   CMMPointer()
   {
      obj=0;
   }
   //Constructing with a pointer
   CMMPointer(T *o)
   {
      obj=0;
      *this=o;
   }
   //Constructing with another smart pointer (copy constructor)
   CMMPointer(const CMMPointer<T> &p)
   {
      obj=0;
		obj = p;
	if(obj)obj->AddRef();
   }

   //Destructor
   ~CMMPointer()
   {
      if(obj)obj->Release();
   }

   //Assignement operators - assigning a plain pointer
   inline T* operator =(T *o)
   {
      if(obj)obj->Release();
      obj=o;
      if(obj)obj->AddRef();
	  return o;
   }
   //Assigning another smart pointer
   inline T* operator =(const CMMPointer<T> &p)
   {
      if(obj)obj->Release();
      obj=p.obj;
      if(obj)obj->AddRef();
	  return obj;
   }

   //Access as a reference
   inline T& operator *() const
   {
      assert(obj!=0 && "Tried to * on a NULL smart pointer");
      return *obj;
   }
   //Access as a pointer
   inline T* operator ->() const
   {
      assert(obj!=0 && "Tried to -> on a NULL smart pointer");
      return obj;
   }

	inline T* getPointer() const
	{
		return obj;
	}


   //Conversion - allow the smart pointer to be automatically
   //converted to type T*
   inline operator T*() const
   {
      return obj;
   }

   inline bool isValid() const
   {
      return (obj!=0);
   }
   inline bool operator !()
   {
      return !(obj);
   }
   inline bool operator ==(const CMMPointer<T> &p) const
   {
      return (obj==p.obj);
   }
   inline bool operator ==(const T* o) const
   {
      return (obj==o);
   }
};



#endif//MEMORY_MANAGEMENT_HH


