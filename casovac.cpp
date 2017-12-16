#include "casovac.h"
#include <time.h>
//#include "pomocne.h"
//#include <Arduino.h>

ct ctAll;

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////// class casovac slouzi ke sposteni udalosti v urcitem intervalu //////////////////////////////////
casovac::casovac ()
{
typ = NONE ;
aktivni = 0;
}
casovac::casovac (unsigned long t,void (*f)(void)) /// INFINITE
{
    typ = INFINITE ;
    p_func = f;
    inkrement = t;
    cas = clock();
    aktivni = 1;
    ctAll.add_address(this);
}
/// ///////////////////////////////////////////////////////////////////////////////////////
casovac::casovac (void (*f)(void))  /// NONE
{
    typ = NONE ;
    p_func = f;
    aktivni = 0;
    ctAll.add_address(this);
}
/// ///////////////////////////////////casovac (time,offset,func) ////////////////////////////////////////////////////////
casovac::casovac (unsigned long t,unsigned long offset,void (*f)(void))  /// INFINITE
{
    p_func = f;
    inkrement = t;
    cas = clock() + offset;
    aktivni = 1;
    ctAll.add_address(this);
}
/// /////////////////////////////   run   ///////////////////////
void casovac::over()
{
    if(clock() > cas )
    {
        cas = cas + inkrement;
        if(aktivni > 0)
        {
            p_func();
            if(typ == ONCE ) aktivni--;
            if(typ == SEVERAL ) citac > 0 ? citac -- : aktivni = 0;
        }
    }
}
/// //////////////////////////////     init    //////////////////
void casovac::init()
{
    cas = clock();
}
/// /////////////////////////////   new cas , new funkce  //////////////////
void casovac::new_time(unsigned long t )
{
    inkrement = t;
}
/// //////////////////////////////////////////////////////////////
void casovac::new_func(void (*f)(void) )
{
    p_func = f;
}
/// ////////////////////// zmena aktivity ///////////////////////
void casovac::aktiv()
{
    aktivni = 1;
}
/// //////////////////////////////////////////////////////////////
void casovac::inaktiv()
{
    aktivni = 0;
}
/// ////////////////////// chekovani timeru /////////////////////
void casovac::loop_all()
{
    ctAll.loop();
}
/// //////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////

ct::ct() {}

/// //////////////////////////   pridani addressy   //////////////////////////////////
void ct::add_address(casovac *addr)
{
    addressy[pocet]  = addr;
    pocet++;
}

/// //////////////////////  spuisteni prikazu clenu v poli     ///////////////////////
void ct::loop(void)
{
    for(int i  = 0; i < pocet; i++)
    {
        addressy[i]->over();
    }
}
/// //////////////////////////// start ONCE ///////////////////////////
void casovac::start(unsigned long t ) /// ONCE
{
    typ = ONCE ;
    cas = clock() + t;
    aktivni++;
}
/// //////////////////////////// start SEVERAL ////////////////////////
void casovac::start(unsigned long t,unsigned long times) /// SEVERAL
{
    typ = SEVERAL ;
    citac = --times;
    inkrement = t;
    cas = clock() + t;
    aktivni++;
}
/// ///////////////////////////////////////////////////////////////////////////
void casovac::start(unsigned long t,unsigned long times,void (*f)(void)) /// SEVERAL
{
    p_func = f;
    typ = SEVERAL ;
    citac = --times;
    inkrement = t;
    cas = clock() + t;
    aktivni++;
}
