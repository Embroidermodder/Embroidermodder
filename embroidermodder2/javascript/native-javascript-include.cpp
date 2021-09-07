#include "native-javascript.h"

//NOTE: http://www.qtcentre.org/threads/20432-Can-I-include-a-script-from-script
//int javaInclude(QString fileName)
//{
//    QFile scriptFile("commands/" + fileName);

//    if(!scriptFile.open(QIODevice::ReadOnly))
//    {
//        return -1;
//    }

//    QTextStream stream(&scriptFile);
//    QString s=stream.readAll();
//    scriptFile.close();

//    QScriptContext* parent=context->parentContext();

//    if(parent!=0)
//    {
//        context->setActivationObject(context->parentContext()->activationObject());
//        context->setThisObject(context->parentContext()->thisObject());
//    }

//    QScriptValue result = engine->evaluate(s);

//    return 0;
//}
