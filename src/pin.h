/*
 ****************************************************************************
 *
 * simulavr - A simulator for the Atmel AVR family of microcontrollers.
 * Copyright (C) 2001, 2002, 2003   Klaus Rudolph		
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ****************************************************************************
 */

#ifndef PIN
#define PIN
#include <string>
using namespace std;

class NetInterface;
class OpenDrain;
class Net;
class UserInterface;

class Pin {
    protected:
        unsigned char *pinOfPort; //points to HWPort::pin or 0L
        unsigned char mask;
        int analogValue;

    public:
        NetInterface *connectedTo;       //only as we calc the net externaly, this must be moved inside pin!!! BUG

        Pin(const OpenDrain &od); 
        typedef enum {
            SHORTED,
            HIGH,
            PULLUP,
            TRISTATE,
            PULLDOWN,
            LOW,
            ANALOG,
            ANALOG_SHORTED
        }T_Pinstate;

        T_Pinstate outState;

    public:
        void SetOutState( T_Pinstate s);
        virtual void SetInState ( const Pin &p);

        Pin(T_Pinstate ps);
        Pin();
        Pin( unsigned char *parentPin, unsigned char mask); 
        operator unsigned char() const;
        virtual Pin &operator= (unsigned char);
        virtual operator bool() const;
        virtual Pin operator+ (const Pin& p);
        virtual Pin operator+= (const Pin& p);
        virtual void RegisterNet(Net *n);
        virtual Pin GetPin() { return *this;}
        virtual ~Pin(){}
        //T_Pinstate GetOutState();
        int GetAnalog() const;

        friend class HWPort;
        friend class Net;

};

class ExternalType {
    public:
        virtual void SetNewValueFromUi(const string &)=0;
};


class ExtPin : public Pin, ExternalType {
    protected:
        UserInterface *ui;
        string extName;

    public:
        void SetNewValueFromUi(const string &);
        ExtPin(T_Pinstate, UserInterface *, char *_extName, char *baseWindow); 
        //Pin &operator= (unsigned char);

        void SetInState(const Pin& p);
};

class ExtAnalogPin : public Pin, ExternalType {
    protected:
        UserInterface *ui;
        string extName;

    public:
        void SetNewValueFromUi(const string &);
        ExtAnalogPin(unsigned int startval, UserInterface *, char *_extName, char* baseWindow); 
        //Pin &operator= (unsigned char);

        void SetInState(const Pin& p);
};

class OpenDrain: public Pin {
    protected:
        Pin *pin;

    public:
        OpenDrain(Pin *p) { pin=p;}
        virtual operator bool() const;
        virtual Pin operator+ (const Pin& p);
        virtual Pin operator+= (const Pin& p);
        virtual Pin GetPin();
        void RegisterNet(Net *n) { pin->RegisterNet(n);}
        virtual ~OpenDrain() {}
};

#endif