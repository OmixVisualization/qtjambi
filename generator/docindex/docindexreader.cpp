#include "docindexreader.h"
#include "docmodel.h"
#include "docclass.h"
#include "reporthandler.h"

DocIndexReader::DocIndexReader()
{

}

void DocIndexReader::analyzeEnum(const QDir& subdir, DocModel* model, QDomElement element){
    if(element.attribute("status")=="active" || element.attribute("status")=="obsolete"){
        DocEnum* cls = new DocEnum(model);
        cls->setSubdir(subdir);
        cls->setName(element.attribute("name"));
        cls->setHref(element.attribute("href"));
        cls->setBrief(element.attribute("brief"));
        cls->setFullName(element.attribute("fullname"));
        model->addEnum(cls);
    }
}

void DocIndexReader::analyzeClass(const QDir& subdir, DocModel* model, QDomElement element){
    if(element.attribute("status")=="active" || element.attribute("status")=="obsolete"){
        DocClass* cls = new DocClass(model);
        cls->setSubdir(subdir);
        cls->setName(element.attribute("name"));
        cls->setHref(element.attribute("href"));
        cls->setBrief(element.attribute("brief"));
        cls->setFullName(element.attribute("fullname"));
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.size(); ++i){
            QDomNode child = childNodes.item(i);
            if(child.isElement()){
                if(child.nodeName()=="class" || child.nodeName()=="struct" || child.nodeName()=="union"){
                    analyzeClass(subdir, model, child.toElement());
                }else if(child.nodeName()=="enum"){
                    analyzeEnum(subdir, model, child.toElement());
                }else if(child.nodeName()=="function"){
                    analyzeFunction(subdir, cls, child.toElement());
                }else if(child.nodeName()=="variable"){
                    analyzeVariable(subdir, cls, child.toElement());
                }else if(child.nodeName()=="property"){
                    analyzeProperty(subdir, cls, child.toElement());
                }
            }
        }
        model->addClass(cls);
    }
}

void DocIndexReader::analyzeNamespace(const QDir& subdir, DocModel* model, QDomElement element){
    if(element.attribute("status")=="active" || element.attribute("status")=="obsolete"){
        QString fullName = element.attribute("fullname");
        if(fullName.isEmpty()){
            fullName = element.attribute("name");
        }
        DocNamespace* ns = model->getNamespace(fullName);
        if(!ns){
            ns = new DocNamespace(model);
            ns->setSubdir(subdir);
            ns->setName(element.attribute("name"));
            ns->setHref(element.attribute("href"));
            ns->setBrief(element.attribute("brief"));
            ns->setFullName(element.attribute("fullname"));
            model->addNamespace(ns);
        }
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.size(); ++i){
            QDomNode child = childNodes.item(i);
            if(child.isElement()){
                if(child.nodeName()=="class" || child.nodeName()=="struct" || child.nodeName()=="union"){
                    analyzeClass(subdir, model, child.toElement());
                }else if(child.nodeName()=="function"){
                    analyzeFunction(subdir, ns, child.toElement());
                }else if(child.nodeName()=="enum"){
                    analyzeEnum(subdir, model, child.toElement());
                }else if(child.nodeName()=="namespace"){
                    analyzeNamespace(subdir, model, child.toElement());
                }
            }
        }
    }
}

void DocIndexReader::analyzeVariable(const QDir& subdir, DocClass* cls, QDomElement element){
    if(element.attribute("status")=="active" || element.attribute("status")=="obsolete"){
        DocVariable* var = new DocVariable(cls);
        var->setSubdir(subdir);
        var->setName(element.attribute("name"));
        var->setType(element.attribute("type"));
        var->setHref(element.attribute("href"));
        var->setBrief(element.attribute("brief"));
        var->setFullName(element.attribute("fullname"));
        var->setStatic(element.attribute("static")=="true");
        cls->addVariable(var);
    }
}

void DocIndexReader::analyzeProperty(const QDir& subdir, DocClass* cls, QDomElement element){
    if(element.attribute("status")=="active" || element.attribute("status")=="obsolete"){
        DocProperty* prop = new DocProperty(cls);
        prop->setSubdir(subdir);
        prop->setName(element.attribute("name"));
        prop->setType(element.attribute("type"));
        prop->setHref(element.attribute("href"));
        prop->setBrief(element.attribute("brief"));
        prop->setFullName(element.attribute("fullname"));
        QString getter;
        QString setter;
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.size(); ++i){
            QDomNode child = childNodes.item(i);
            if(child.isElement()){
                if(child.nodeName()=="getter"){
                    getter = child.toElement().attribute("name");
                }else if(child.nodeName()=="setter"){
                    setter = child.toElement().attribute("name");
                }
            }
        }
        prop->setGetter(getter);
        prop->setSetter(setter);
        cls->addProperty(prop);
    }
}

template<class DocFunctionOwner>
void DocIndexReader::analyzeFunction(const QDir& subdir, DocFunctionOwner* owner, QDomElement element){
    if(element.attribute("status")=="active" || element.attribute("status")=="obsolete"){
        DocFunction* fun = new DocFunction(owner);
        fun->setSubdir(subdir);
        fun->setName(element.attribute("name"));
        fun->setType(element.attribute("type"));
        fun->setHref(element.attribute("href"));
        fun->setBrief(element.attribute("brief"));
        fun->setFullName(element.attribute("fullname"));
        fun->setConst(element.attribute("const")=="true");
        fun->setStatic(element.attribute("static")=="true");
        QStringList parameters;
        QDomNodeList childNodes = element.childNodes();
        for(int i=0; i<childNodes.size(); ++i){
            QDomNode child = childNodes.item(i);
            if(child.isElement()){
                if(child.nodeName()=="parameter"){
                    parameters << child.toElement().attribute("type");
                }
            }
        }
        fun->setParameters(parameters);
        owner->addFunction(fun);
    }
}

const DocModel* DocIndexReader::readDocIndexes(const QDir& docDirectory){
    ReportHandler::setContext("DocIndexReader");
    QScopedPointer<DocModel> model(new DocModel());
    if(docDirectory.exists()){
        for(const QString& subdirp : docDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
            QDir subdir(docDirectory.absoluteFilePath(subdirp));
            for(const QString& idxfilep : subdir.entryList({"*.index"}, QDir::Files)){
                QFile idxfile(subdir.absoluteFilePath(idxfilep));
                if (!idxfile.open(QIODevice::ReadOnly))
                    continue;
                QDomDocument doc;
                if (!doc.setContent(&idxfile)) {
                    idxfile.close();
                    continue;
                }
                idxfile.close();

                QDomElement docElem = doc.documentElement();
                if(docElem.tagName()=="INDEX"){
                    QDomNodeList childNodes = docElem.childNodes();
                    for(int i=0; i<childNodes.size(); ++i){
                        QDomNode child = childNodes.item(i);
                        if(child.isElement()){
                            if(child.nodeName()=="namespace"){
                                analyzeNamespace(subdir, model.get(), child.toElement());
                            }
                        }
                    }
                }
            }
        }
    }
    if(model->isEmpty()){
        model.reset();
    }
    return model.take();
}
