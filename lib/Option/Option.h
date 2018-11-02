#ifndef OPTION_H
#define OPTION_H

//Please let this link here for the creator to understand what's going on here
//https://stackoverflow.com/questions/12009314/how-to-create-an-array-of-templated-class-objects

struct AbstractOption{
    virtual ~AbstractOption() = 0;
};

template <class type>
class Option : public AbstractOption
{
    public:
        Option(const char *name, bool is_const=false);
        void setValue(type newValue);
        type getValue();
        //TODO: getDispStr()
        ~Option();
        const char *_name; //Display name
    private:
        type *_value; //Value of the option
        bool _is_const; //Is the option a constant
};

#endif