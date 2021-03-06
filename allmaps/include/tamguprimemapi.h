/*
 *  Tamgu (탐구)
 *
 * Copyright 2019-present NAVER Corp.
 * under BSD 3-clause
 */
/* --- CONTENTS ---
 Project    : Tamgu (탐구)
 Version    : See tamgu.cxx for the version number
 filename   : tamguprimemapi.h
 Date       : 2017/09/01
 Purpose    : map implementation 
 Programmer : Claude ROUX (claude.roux@naverlabs.com)
 Reviewer   :
*/

#ifndef Tamguprimemapi_h
#define Tamguprimemapi_h

#include "fractalhmap.h"
#include "tamguint.h"
#include "tamgustring.h"
#include "tamguvector.h"
#include "tamguconstants.h"
#include "tamguivector.h"

//We create a map between our methods, which have been declared in our class below. See MethodInitialization for an example
//of how to declare a new method.
class Tamguprimemapi;
//This typedef defines a type "primemapiMethod", which expose the typical parameters of a new Tamgu method implementation
typedef Tamgu* (Tamguprimemapi::*primemapiMethod)(Tamgu* contextualpattern, short idthread, TamguCall* callfunc);
//---------------------------------------------------------------------------------------------------------------------
class Tamguprimemapi : public TamguObjectLockContainer {
    public:
    //We export the methods that will be exposed for our new object
    //this is a static object, which is common to everyone
    //We associate the method pointers with their names in the linkedmethods map
    static Exchanging basebin_hash<primemapiMethod> methods;
    static Exchanging hmap<string, string> infomethods;
    static Exchanging bin_hash<unsigned long> exported;

    static Exchanging short idtype;

    //---------------------------------------------------------------------------------------------------------------------
    //This SECTION is for your specific implementation...
    //Your personal variables here...
    prime_hash<long, Tamgu*> values;
    bool isconst;

    //---------------------------------------------------------------------------------------------------------------------
    Tamguprimemapi(TamguGlobal* g, Tamgu* parent = NULL) : TamguObjectLockContainer(g, parent) {
        //Do not forget your variable initialisation
        isconst = false;
    }

    Tamguprimemapi() {
        //Do not forget your variable initialisation
        isconst = false;
    }

    //----------------------------------------------------------------------------------------------------------------------
    Exporting Tamgu* Loopin(TamguInstruction*, Tamgu* context, short idthread);
    Exporting Tamgu* Put(Tamgu* index, Tamgu* value, short idthread);
    Exporting Tamgu* Eval(Tamgu* context, Tamgu* value, short idthread);

    void SetConst() { isconst = true;}

    short Type() {
        return Tamguprimemapi::idtype;
    }

    string Typename() {
        return "primemapi";
    }

    bool isObjectContainer() {
        return true;
    }

    bool isContainerClass() {
        return true;
    }

    bool isMapContainer() {
        return true;
    }

    bool duplicateForCall() {
        return isconst;
    }

    void Setprotect(bool n) {
        if (loopmark)
            return;
        loopmark=true;
        protect = n;
        prime_hash<long, Tamgu*>::iterator it;

        Locking* _lock = _getlock(this);
        for (it = values.begin(); it != values.end(); it++)
            it->second->Setprotect(n);
        _cleanlock(_lock);
        
        loopmark=false;
    }

    void Popping() {
        if (loopmark)
            return;
        loopmark=true;
        protect = false;
        if (Reference() <= 0)
            protect = true;
        prime_hash<long, Tamgu*>::iterator it;

        Locking* _lock = _getlock(this);
        for (it = values.begin(); it != values.end(); it++)
            it->second->Popping();
        _cleanlock(_lock);
        
        loopmark=false;
    }

    void Protectcontainervalues() {
        if (loopmark)
            return;
        loopmark=true;
        protect = true;
        prime_hash<long, Tamgu*>::iterator it;
        
        Locking* _lock = _getlock(this);
        for (it = values.begin(); it != values.end(); it++)
            it->second->Setprotect(true);
        _cleanlock(_lock);
        
        loopmark=false;
    }


    Tamgu* Atom(bool forced) {
        if (forced) {
            Tamguprimemapi* m = new Tamguprimemapi;
            Locking _lock(this);
            Tamgu* v;
            prime_hash<long, Tamgu*>::iterator it;
            for (it = values.begin(); it != values.end(); it++) {
                v = it->second->Atom(true);
                m->values[it->first] = v;
                v->Setreference();
            }
            return m;
        }
        return this;
    }

    double Sum() {
        Locking* _lock = _getlock(this);
        double v = 0;
        prime_hash<long, Tamgu*>::iterator itx;
        for (itx = values.begin(); itx != values.end(); itx++)
            v += itx->second->Sum();
        _cleanlock(_lock);
        return v;
    }

    double Product() {
        Locking* _lock = _getlock(this);
        double v = 1;
        prime_hash<long, Tamgu*>::iterator itx;
        for (itx = values.begin(); itx != values.end(); itx++)
            v *= itx->second->Product();
        _cleanlock(_lock);
        return v;
    }
    
    //---------------------------------------------------------------------------------------------------------------------
    //Declaration
    //All our methods must have been declared in tamguexportedmethods... See MethodInitialization below
    bool isDeclared(short n) {
        if (exported.find(n) != exported.end())
            return true;
        return false;
    }

    
    Tamgu* Newvalue(Tamgu* a, short idthread) {
        Tamguprimemapi* m = new Tamguprimemapi;
        if (a->isContainer()) {
            TamguIteration* it = a->Newiteration(false);
            for (it->Begin(); it->End() == aFALSE; it->Next()) {
                m->Push(it->Keyinteger(),it->IteratorValue());
            }
            it->Release();
            return m;
        }
        
        prime_hash<long,Tamgu*>::iterator ist;
        for (ist=values.begin(); ist!=values.end();ist++)
            m->Push(ist->first,a);
        return m;
    }

    Tamgu* Newinstance(short idthread, Tamgu* f = NULL) {
        return new Tamguprimemapi();
    }

    Exporting TamguIteration* Newiteration(bool direction);


    static void AddMethod(TamguGlobal* global, string name, primemapiMethod func, unsigned long arity, string infos);
    static bool InitialisationModule(TamguGlobal* global, string version);

    void Methods(Tamgu* v) {
        hmap<string, string>::iterator it;
        for (it = infomethods.begin(); it != infomethods.end(); it++)
            v->storevalue(it->first);
    }

    string Info(string n) {

        if (infomethods.find(n) != infomethods.end())
            return infomethods[n];
        return "Unknown method";
    }



    //---------------------------------------------------------------------------------------------------------------------
    
    void unmark() {
        Locking _lock(this);
        if (loopmark)
            return;
            
        loopmark=true;
        usermark=false;

        prime_hash<long,Tamgu*>::iterator it;
        for (it=values.begin();it!=values.end();it++)
            it->second->unmark();
            
        loopmark=false;
    }

    Exporting void Cleanreference(short inc);

    Exporting void Setreference(short r);
    Exporting void Setreference();
    Exporting void Resetreference(short r = 1);
    //---------------------------------------------------------------------------------------------------------------------
    //This SECTION is for your specific implementation...
    //This is an example of a function that could be implemented for your needs.

    
    Tamgu* MethodInvert(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        if (contextualpattern == this || !contextualpattern->isMapContainer() || !contextualpattern->isAffectation())
            contextualpattern = Newinstance(idthread);
        else
            contextualpattern->Clear();
            
        Tamgu* a;
        prime_hash<long, Tamgu*>::iterator it;
        for (it = values.begin(); it != values.end(); it++) {
            a = globalTamgu->Provideint(it->first);
            contextualpattern->Push(it->second, a);
            a->Release();
        }

        return contextualpattern;
    }

    Exporting Tamgu* MethodFind(Tamgu* contextualpattern, short idthread, TamguCall* callfunc);


    Tamgu* MethodClear(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Clear();
        return aTRUE;
    }


    
    Tamgu* MethodItems(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        prime_hash<long, Tamgu*>::iterator itr;
        Tamguprimemapi* item;
        Tamgu* vect = Selectavector(contextualpattern);
        for (itr=values.begin(); itr!=values.end();itr++) {
            item=new Tamguprimemapi;
            item->Push(itr->first, itr->second);
            vect->Push(item);
        }
        return vect;
    }
    
    Tamgu* MethodMerge(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {

        
        Tamgu* v = callfunc->Evaluate(0, contextualpattern, idthread);
        Merging(v);
        return this;
    }

    Tamgu* MethodSum(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        double v = Sum();
        return globalTamgu->Providefloat(v);
    }

    Tamgu* MethodKeys(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        Tamguivector* vstr = (Tamguivector*)Selectaivector(contextualpattern);
        prime_hash<long, Tamgu*>::iterator it;
        for (it = values.begin(); it != values.end(); it++)
            vstr->values.push_back(it->first);
        return vstr;
    }

    Tamgu* MethodValues(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        Tamguvector* v = (Tamguvector*)Selectavector(contextualpattern);
        prime_hash<long, Tamgu*>::iterator it;
        for (it = values.begin(); it != values.end(); it++)
            v->Push(it->second);
        return v;
    }

    Tamgu* MethodTest(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        long  v = callfunc->Evaluate(0, contextualpattern, idthread)->Integer();
        if (values.find(v) == values.end())
            return aFALSE;
        return aTRUE;
    }

    Tamgu* MethodProduct(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        double v = Product();
        return globalTamgu->Providefloat(v);
    }

    Tamgu* MethodPop(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        Tamgu* pos = callfunc->Evaluate(0, contextualpattern, idthread);
        return Pop(pos);
    }

    Tamgu* MethodJoin(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        //The separator between keys
        string keysep = callfunc->Evaluate(0, contextualpattern, idthread)->String();
        //The separator between values
        string sep = callfunc->Evaluate(1, contextualpattern, idthread)->String();
        prime_hash<long, Tamgu*>::iterator it;
        bool beg = true;
        stringstream res;
        for (it = values.begin(); it != values.end(); it++) {
            if (beg == false)
                res << sep;
            beg = false;
            res << it->first << keysep << it->second->String();
        }

        return globalTamgu->Providestring(res.str());
    }


    //------------------------------------------------------------------------------------------
    Exporting Tamgu* Push(Tamgu* k, Tamgu* v);
    
    
    Exporting Tamgu* Pop(Tamgu* k);
    Tamgu* Push(long k, Tamgu* a) {

        Locking _lock(this);
        if (values.find(k) != values.end()) {
            Tamgu* v = values[k];
            v->Removereference(reference + 1);
        }
        a = a->Atom();
        values[k] = a;
        a->Addreference(reference + 1);
        return this;
    }

    Tamgu* Combine(Tamgu* ke) {
        //Three cases:
        if (!ke->isContainer())
            return this;
        Doublelocking(this, ke);
        Tamguvector* v;


        TamguIteration* itr = ke->Newiteration(false);
        for (itr->Begin(); itr->End() == aFALSE; itr->Next()) {
            long n = itr->Keyinteger();
            if (values.find(n) == values.end())
                Push(n, itr->Value());
            else {
                v = globalTamgu->Providevector();
                v->Push(values[n]);
                v->Push(itr->Value());
                Push(n, v);
            }
        }
        itr->Release();
        return this;
    }

    Tamgu* Merging(Tamgu* ke) {
        //Three cases:
        if (!ke->isContainer())
            return this;
        Doublelocking(this, ke);


        TamguIteration* itr = ke->Newiteration(false);
        for (itr->Begin(); itr->End() == aFALSE; itr->Next()) {
            long n = itr->Keyinteger();
            if (values.find(n) == values.end())
                Push(n, itr->Value());
        }
        itr->Release();
        return this;
    }

    //---------------------------------------------------------------------------------------------------------------------

    //ExecuteMethod must be implemented in order to execute our new Tamgu methods. This method is called when a TamguCallMethodMethod object
    //is returned by the Declaration method.
    Tamgu* CallMethod(short idname, Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        //This call is a bit cryptic. It takes the method (function) pointer that has been associated in our map with "name"
        //and run it with the proper parameters. This is the right call which should be invoked from within a class definition
        return (this->*methods.get(idname))(contextualpattern, idthread, callfunc);
    }

    Exporting void Clear();
    

    Exporting string String();
    Exporting string JSonString();

    Tamgu* Value(string n) {
        long v = convertlong(n);
        Locking _lock(this);
        if (values.find(v) == values.end())
            return aNOELEMENT;
        return values[v];
    }

    Tamgu* Value(Tamgu* a) {
        long n =  a->Integer();

        Locking _lock(this);
        if (values.find(n) == values.end())
            return aNOELEMENT;
        return values[(long)n];
    }

    Tamgu* Value(long n) {
        Locking _lock(this);
        if (values.find(n) == values.end())
            return aNOELEMENT;
        return values[(long)n];
    }

    Tamgu* Value(double n) {
        Locking _lock(this);
        if (values.find(n) == values.end())
            return aNOELEMENT;
        return values[(long)n];
    }

    Exporting long Integer();
    Exporting double Float();
    Exporting BLONG Long();
    Exporting bool Boolean();


    //Basic operations
    Exporting long Size();
    
    Exporting Tamgu* in(Tamgu* context, Tamgu* a, short idthread);

    Exporting Tamgu* andset(Tamgu* a, bool itself);
    Exporting Tamgu* orset(Tamgu* a, bool itself);
    Exporting Tamgu* xorset(Tamgu* a, bool itself);
    Exporting Tamgu* plus(Tamgu* a, bool itself);
    Exporting Tamgu* minus(Tamgu* a, bool itself);
    Exporting Tamgu* multiply(Tamgu* a, bool itself);
    Exporting Tamgu* divide(Tamgu* a, bool itself);
    Exporting Tamgu* power(Tamgu* a, bool itself);
    Exporting Tamgu* shiftleft(Tamgu* a, bool itself);
    Exporting Tamgu* shiftright(Tamgu* a, bool itself);
    Exporting Tamgu* mod(Tamgu* a, bool itself);

    Exporting Tamgu* same(Tamgu* a);

};

//--------------------------------------------------------------------------------------------------
class TamguIterationprimemapi : public TamguIteration {
    public:

    prime_hash<long, Tamgu*>::iterator it;
    Tamguprimemapi* map;
    Locking _lock;

    TamguIterationprimemapi(Tamguprimemapi* m, bool d, TamguGlobal* g = NULL) : map(m), _lock(m), TamguIteration(d, g) {}

    
    void Setvalue(Tamgu* recipient, short idthread) {
        recipient->storevalue(it->first);
    }

    Tamgu* Key() {
        return globalTamgu->Provideint(it->first);
    }

    Tamgu* Value() {
        return it->second;
    }

    string Keystring() {
        
        return convertfromnumber(it->first);
        
    }

    string Valuestring() {
        return it->second->String();
    }

    wstring Keyustring() {
        
        return wconvertfromnumber(it->first);
        
    }

    wstring Valueustring() {
        return it->second->UString();
    }

    
    BLONG Keylong() {
        return it->first;
    }

    long Keyinteger() {
        return it->first;
    }

    
    BLONG Valuelong() {
        return it->second->Long();
    }

    long Valueinteger() {
        return it->second->Float();
    }

    double Keyfloat() {
        return it->first;
    }

    double Valuefloat() {
        return it->second->Float();
    }

    void Next() {
        it++;
    }

    Tamgu* End() {
        if (it == map->values.end())
            return aTRUE;
        return aFALSE;
    }

    Tamgu* Begin() {
        it = map->values.begin();
        return aTRUE;
    }

};

//-------------------------------------------------------------------------------------------

#endif
