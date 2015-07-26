To know what Boost.Object is all about, please read through the [Documentation](http://andreasbrueck.github.io/BoostObject). If you ever used Qt, you will notice alot of similarities. As an example, here is an object derived from BObject in contrast to an object derived from QObject:

<table>
<tr>
<td><b>Boost.Object</b></td>
<td><b>Qt</b></td>
</tr>
<tr>
<td>
<pre lang="cpp">
class myObject : public BObject
{
public:
    myObject(BObject * parent = 0)
        :
          BObject(parent)
    {
        mySignal.connect(mySlot);
    }
    
private:
    void _slot_mySlot(int value)
    {
        // do something with value
    }
    
public:
    auto_slot(mySlot)
    bsignal(mySignal,void(int))
};
</pre>
</td>
<td>
<pre lang="cpp">
class myObject : public QObject
{
public:
    myObject(QObject * parent = 0)
        :
          QObject(parent)
    {
        connect(this,SIGNAL(mySignal),SLOT(mySlot));
    }
    
public slots:
    void mySlot(int value)
    {
        // do something with value
    }
    
signals:
    void mySignal(int);
};
</pre>
</td>
</tr>
</table>

But beware! There are fundamental differences between Boost.Object and Qt. The documentation has a short list [here](http://andreasbrueck.github.io/BoostObject/libs/object/doc/html/index.html#boost_object.introduction.qt_similarities_and_differences). In general, do not assume that Boost.Object and Qt are the same just because things look similar.

### Status
System | Status
--- | ---
Linux g++ 4.9.2 | [![Build Status](https://travis-ci.org/andreasbrueck/BoostObject.svg)](https://travis-ci.org/andreasbrueck/BoostObject)
Visual Studio 2013 | [![Build status](https://ci.appveyor.com/api/projects/status/ha6ucwh4tn8kt095?svg=true)](https://ci.appveyor.com/project/andreasbrueck/boostobject)

### Building/Installing
Boost.Object is header only! All you have to do is copy the *boost* directory to your local boost folder, and you're good to go. 
### Contributing
I am happy to hear about ideas for this library, or receive bug fixes. Just create an issue or pull request on master.

### Questions
If you have any questions, please create an issue.

---

Boost.Object is neither an official nor an inofficial boost library. It builds upon boost. Its folder structure is kept incase it upgrades to "inofficial" at some time in the future.