#include "mymsg.h"
#include <QMessageBox>

void ItemNotExistMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("البند غير موجود");
    db_msg.setText("هذا البند غير موجود في قاعده البينات");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void LogOutMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("تسجيل الخروج");
    db_msg.setText("تم تسجيل الخروج بنجاح");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddItemEmptyMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Critical);
    db_msg.setWindowTitle("البينات ناقصه");
    db_msg.setText("كل الحقول مطلوبه الرجاء تعبئتها جميعا");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void DBFailMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Critical);
    db_msg.setWindowTitle("خطا في الاتصال");
    db_msg.setText("يوجد خطا في الاتصال بقاعده البينات");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ItemExistMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Critical);
    db_msg.setWindowTitle("هذا البند موجود");
    db_msg.setText("يوجد بالفعل بند بنفس رقم الباركود او الاسم");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddItemSuccessfullyMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("العمليه بنجاح");
    db_msg.setText("تم اضافة الصنف بنجاج");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void EditItemSuccessfullyMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("العمليه بنجاح");
    db_msg.setText("تم تحديث الصنف بنجاج");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddEmployeeMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("العمليه بنجاح");
    db_msg.setText("تم اضافة المستخدم بنجاح");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void CantDiscountMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("فشل التخفيض");
    db_msg.setText("قيمه التخفيض اكبر من قيمة البند");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddBillExpireMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("فشل الاضافه");
    db_msg.setText("لايمكن اضافة فاتوره ذات تاريخ صلاحيه منتهي!");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddBillSuccessfullyMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("العمليه بنجاح");
    db_msg.setText("تم اضافة الفاتورة بنجاج");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void EditBillSuccessfullyMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("العمليه بنجاح");
    db_msg.setText("تم تحديث الفاتورة بنجاج");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ReadBillFailedMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText("لايمكن ايجاد الفاتوره لهذا المنتج");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void NotEnoughAmountMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText("مخزون هذا المنتج قد انتهي");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AmountOverFlowMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("فشل الاضافه");
    db_msg.setText("لقد ادخلت كميه اكبر من المتوفره");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ClientNameNotVaildMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("فشل الاضافه");
    db_msg.setText("اسم العميل او قيمة الكوبون لايمكن ان يكون فارغ");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void CartIdNotExistMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText("لاتوجد اصناف تم بيعها تحت رقم الفاتوره هذا");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ItemNotExistOnCartIDMsg(QString item)
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText(QString("هذا المنتج غير موجود لهذه الفاتوره: '%1' او تحقق من النوع (بيع-شراء-جمله) يجب ان يكون مطابقا له او الكميه او التخفيض").arg(item));
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ClientHasNoBillLeft()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("فارغ");
    db_msg.setText("هذا العميل ليس لديه اي فواتير غير مدفوعه");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ClientFinishPaymentMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("تمت العمليه");
    db_msg.setText("تم سداد الفاتوره بنجاح");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void ClientBillAboveTotalPriceMsg()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Warning);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText("المبلغ الذي ادخلته اكبر من المبلغ المتبقي لهذه الفاتوره");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void SupplierNotExist()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Critical);
    db_msg.setWindowTitle("البينات خاطئه");
    db_msg.setText("تحقق من رقم الفاتورة او اسم المورد");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddShotItemSuccessfully()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("تمت العمليه");
    db_msg.setText("تم اضافة المنتجات الي قسم التوالف");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void AddExpensesSuccessfully()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("تمت العمليه");
    db_msg.setText("تم الاضافة بنجاح");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void CobonGraterTotal()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Critical);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText("قيمة الكوبون المدخله اكبر من او تساوي قيمة الاصناف");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

void PaidGraterCash()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Critical);
    db_msg.setWindowTitle("حصل خطا");
    db_msg.setText("قيمة الدفع التي ادخلتها اكبر من قيمة الدين الموجوده");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}

bool YesNoMsg(QString title)
{
    QMessageBox messageBox(QMessageBox::Question,title,"هل تريد تاكيد العمليه؟",QMessageBox::Yes|QMessageBox::No);
    messageBox.setButtonText(QMessageBox::Yes, "موافق");
    messageBox.setButtonText(QMessageBox::No,  "الغاء");
    if (messageBox.exec() == QMessageBox::Yes)
        return true;
    return false;
}

void ConfigUpdated()
{
    QMessageBox db_msg;

    db_msg.setIcon(QMessageBox::Information);
    db_msg.setWindowTitle("تم التحديث");
    db_msg.setText("تم تحديث الاعدادات");
    db_msg.setStandardButtons(QMessageBox::Close);
    db_msg.setButtonText(QMessageBox::Close,"انهاء");
    db_msg.exec();
}
