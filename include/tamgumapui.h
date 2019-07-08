/*
 *  Tamgu (탐구)
 *
 * Copyright 2019-present NAVER Corp.
 * under BSD 3-clause
 */
/* --- CONTENTS ---
 Project    : Tamgu (탐구)
 Version    : See tamgu.cxx for the version number
 filename   : tamgumapui.h
 Date       : 2017/09/01
 Purpose    : map implementation 
 Programmer : Claude ROUX (claude.roux@naverlabs.com)
 Reviewer   :
*/

#ifndef Tamgumapui_h
#define Tamgumapui_h

//#prime
#include "tamguint.h"
#include "tamgustring.h"
#include "tamguvector.h"
#include "tamguconstants.h"
#include "tamguuvector.h"
#include "tamguivector.h"

//We create a map between our methods, which have been declared in our class below. See MethodInitialization for an example
//of how to declare a new method.
class Tamgumapui;
//This typedef defines a type "mapuiMethod", which expose the typical parameters of a new Tamgu method implementation
typedef Tamgu* (Tamgumapui::*mapuiMethod)(Tamgu* contextualpattern, short idthread, TamguCall* callfunc);
//---------------------------------------------------------------------------------------------------------------------
class Tamgumapui : public TamguLockContainer {
    public:
    //We export the methods that will be exposed for our new object
    //this is a static object, which is common to everyone
    //We associate the method pointers with their names in the linkedmethods map
    static Exchanging basebin_hash<mapuiMethod> methods;
    static Exchanging hmap<string, string> infomethods;
    static Exchanging bin_hash<unsigned long> exported;

    static Exchanging short idtype;

    //---------------------------------------------------------------------------------------------------------------------
    //This SECTION is for your specific implementation...
    //Your personal variables here...
    hmap<wstring, long> values;
    bool isconst;

    //---------------------------------------------------------------------------------------------------------------------
    Tamgumapui(TamguGlobal* g, Tamgu* parent = NULL) : TamguLockContainer(g, parent) {
        //Do not forget your variable initialisation
        isconst = false; 

    }

    Tamgumapui() {
        //Do not forget your variable initialisation
        isconst = false; 

    }

    //----------------------------------------------------------------------------------------------------------------------
    Exporting Tamgu* Loopin(TamguInstruction* ins, Tamgu* context, short idthread);
    Exporting Tamgu* Put(Tamgu* index, Tamgu* value, short idthread);
    Exporting Tamgu* Get(Tamgu* context, Tamgu* value, short idthread);

    void SetConst() { isconst = true;}

    short Type() {
        return Tamgumapui::idtype;
    }

    

    static void Setidtype(TamguGlobal* global);
    
    string Typename() {
        return "mapui";
    }

    bool isNumber() {
        return true;
    }

    bool isContainerClass() {
        return true;
    }

    bool isMapContainer() {
        return true;
    }

    bool isValueContainer() {
        return true;
    }

    Tamgu* Atom(bool forced) {
        if (forced || !protect || reference) {
            Locking _lock(this);
            Tamgumapui * m = new Tamgumapui;
            m->values = values;
            return m;
        }
        return this;
    }

    double Sum() {
        Locking* _lock = _getlock(this);
        double v = 0;
        hmap<wstring, long>::iterator itx;
        for (itx = values.begin(); itx != values.end(); itx++)
            v += itx->second;
        _cleanlock(_lock);
        return v;
    }

    double Product() {
        Locking* _lock = _getlock(this);
        double v = 1;

        for (auto& itx : values)
            v *= itx.second;
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
        Tamgumapui* m = new Tamgumapui;
        if (a->isContainer()) {
            TamguIteration* it = a->Newiteration(false);
            for (it->Begin(); it->End() == aFALSE; it->Next()) {
                m->values[it->Keyustring()]=it->Valueinteger();
            }
            it->Release();
            return m;
        }
        
        long val=a->Integer();
        hmap<wstring,long>::iterator ist;
        for (ist=values.begin(); ist!=values.end();ist++)
            m->values[ist->first]=val;
        return m;
    }

    Tamgu* Newinstance(short idthread, Tamgu* f = NULL) {
        return new Tamgumapui();
    }

    Exporting TamguIteration* Newiteration(bool direction);

    bool duplicateForCall() {
        return isconst;
    }




    static void AddMethod(TamguGlobal* global, string name, mapuiMethod func, unsigned long arity, string infos);
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
    //This SECTION is for your specific implementation...
    //This is an example of a function that could be implemented for your needs.

    
    Tamgu*
    MethodInvert(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        if (contextualpattern == this || !contextualpattern->isMapContainer() || !contextualpattern->isAffectation())
            contextualpattern = Newinstance(idthread);
        else
            contextualpattern->Clear();

        Tamgu* a;

        for (auto& it : values) {
            a = globalTamgu->Provideustring(it.first);
            contextualpattern->Push(it.second, a);
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

        Tamgumapui* item;
        Tamgu* vect = Selectavector(contextualpattern);
        for (auto& itr : values) {
            item=new Tamgumapui;
            item->values[itr.first] = itr.second;
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
        Tamguuvector* vstr = (Tamguuvector*)Selectauvector(contextualpattern);
        hmap<wstring, long>::iterator it;
        for (it = values.begin(); it != values.end(); it++)
            vstr->values.push_back(it->first);
        return vstr;
    }

    Tamgu* MethodValues(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        Tamguivector * vstr = (Tamguivector*)Selectaivector(contextualpattern);
        hmap<wstring, long>::iterator it;
        for (it = values.begin(); it != values.end(); it++)
            vstr->values.push_back(it->second);
        return vstr;
    }

    Tamgu* MethodTest(Tamgu* contextualpattern, short idthread, TamguCall* callfunc) {
        Locking _lock(this);
        wstring  v = callfunc->Evaluate(0, contextualpattern, idthread)->UString();
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
        wstring keysep = callfunc->Evaluate(0, contextualpattern, idthread)->UString();
        //The separator between values
        wstring sep = callfunc->Evaluate(1, contextualpattern, idthread)->UString();

        bool beg = true;
        std::wstringstream res;
        for (auto& it : values) {
            if (beg == false)
                res << sep;
            beg = false;
            res << it.first << keysep << it.second;
        }

        return globalTamgu->Provideustring(res.str());
    }


    //------------------------------------------------------------------------------------------
    Exporting Tamgu* Push(Tamgu* k, Tamgu* v);
    Exporting Tamgu* Pop(Tamgu* kkey);

    Tamgu* Push(wstring k, Tamgu* a) {
        Locking _lock(this);
        values[k] = a->Integer();
        return this;
    }

    Tamgu* Combine(Tamgu* ke) {
        //Three cases:
        if (!ke->isContainer())
            return this;
        Doublelocking(this, ke);


        TamguIteration* itr = ke->Newiteration(false);
        for (itr->Begin(); itr->End() == aFALSE; itr->Next()) {
            wstring n = itr->Keyustring();
            if (values.find(n) == values.end())
                values[n] = itr->Valueinteger();
            else
                values[n] += itr->Valueinteger();
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
            wstring n = itr->Keyustring();
            if (values.find(n) == values.end())
                values[n] = itr->Valueinteger();
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

    Tamgu* Value(Tamgu* a) {
        wstring n =  a->UString();

        Locking _lock(this);
        if (values.find(n) == values.end())
            return aNOELEMENT;
        return globalTamgu->Provideint(values[n]);
    }

    Tamgu* Value(wstring n) {
        Locking _lock(this);
        if (values.find(n) == values.end())
            return aNOELEMENT;
        return globalTamgu->Provideint(values[n]);
    }

    Tamgu* Value(string s) {
        wstring n;
        s_utf8_to_unicode(n, USTR(s), s.size());
        Locking _lock(this);
        if (values.find(n) == values.end())
            return aNOELEMENT;
        return globalTamgu->Provideint(values[n]);
    }

    Tamgu* Value(long n) {
        
        wstring s = wconvertfromnumber(n);
        Locking _lock(this);
        if (values.find(s) == values.end())
            return aNOELEMENT;
        return globalTamgu->Provideint(values[s]);
    }

    Tamgu* Value(double n) {
        
        wstring s = wconvertfromnumber(n);
        Locking _lock(this);
        if (values.find(s) == values.end())
            return aNOELEMENT;
        return globalTamgu->Provideint(values[s]);
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
class TamguIterationmapui : public TamguIteration {
    public:

    hmap<wstring, long>::iterator it;
    Tamgumapui map;

    TamguIterationmapui(Tamgumapui* m, bool d, TamguGlobal* g = NULL) : TamguIteration(d, g) {
        map.values = m->values;
    }

    
    void Setvalue(Tamgu* recipient, short idthread) {
        recipient->storevalue(it->first);
    }

    Tamgu* Key() {
        return globalTamgu->Provideustring(it->first);
    }

    wstring Keyustring() {
        return it->first;
    }

    string Keystring() {
        string res;
        wstring w = it->first;
        s_unicode_to_utf8(res, w);
        return res;
    }

    long Keyinteger() {
        return convertinteger(it->first);
    }

    double Keyfloat() {
        return convertfloat(it->first);
    }

    
    

    Tamgu* Value() {
        return globalTamgu->Provideint(it->second);
    }

    string Valuestring() {
        
        return convertfromnumber(it->second);
        
    }

    wstring Valueustring() {
        
        return wconvertfromnumber(it->second);
        
    }

    
    BLONG Valuelong() {
        return it->second;
    }

    long Valueinteger() {
        return it->second;
    }

    double Valuefloat() {
        return it->second;
    }

    void Next() {
        it++;
    }

    Tamgu* End() {
        if (it == map.values.end())
            return aTRUE;
        return aFALSE;
    }

    Tamgu* Begin() {
        it = map.values.begin();
        return aTRUE;
    }

};


//-------------------------------------------------------------------------------------------

#endif
