#ifndef __Action__h__
#define __Action__h__

#include "Animation.h"

typedef std::function<double (const double, const class Action*)> ActionRetimeFunction;

class Action {
    public:
        Action(ActionRetimeFunction tmFkt=timeIdentity<Action>) : timeFunction(tmFkt) { activated = false; }

        inline bool wasActivated() const { return activated; }
        virtual void reset(bool soft=false)  { 
            activated = false;
        }
        inline void execute(double time, uint8_t layerMask, LEDState::LayerTypes layer = LEDState::LayerTypes::FinalComposit) {
            if (!activated) {
                activated = true;
                executeIntern(timeFunction(time, this), layerMask, layer);
            }
        }

        const ActionRetimeFunction timeFunction;
    protected:
        virtual void executeIntern(double time, uint8_t layerMask, LEDState::LayerTypes layer) = 0;

    private:
        bool activated;
        
};

class DefaultAction : public Action {
    protected:
        typedef std::function<void (const double, const class Action*)> Executable;
    public:

        DefaultAction(Executable act, ActionRetimeFunction tmFkt=timeIdentity<Action>) : Action(tmFkt), exec(act) {}
    protected:
        virtual void executeIntern(double time, uint8_t layerMask, LEDState::LayerTypes layer){
            exec(time, this);
        }

    private:
        const Executable exec;
};

class RepeatableAction : public DefaultAction {
    public:
        virtual void reset(bool soft=false)  { 
            if (!soft) {
                DefaultAction::reset();
            }
            rCount = repeat;
        }

        RepeatableAction(DefaultAction::Executable act, int repeat, ActionRetimeFunction tmFkt=timeIdentity<Action>) : DefaultAction(act, tmFkt),  repeat(repeat) {}
    protected:
        virtual void executeIntern(double time, uint8_t layerMask, LEDState::LayerTypes layer){
            if ((rCount--)>0){
                DefaultAction::executeIntern(time, layerMask, layer);
            } 
        }

    private:
        int rCount;
        const int repeat;
};

class TestAction : public Action {
    public:
        TestAction(std::string n,  ActionRetimeFunction tmFkt=timeIdentity<Action>) : Action(tmFkt), name(n){}

    protected:
        virtual void executeIntern(double time, uint8_t layerMask, LEDState::LayerTypes layer){
            Serial.printf("    - Action %s: %d,%d @ %f\n", name.c_str(), layer, layerMask, time);
        }

    private:
        std::string name;
};

#endif