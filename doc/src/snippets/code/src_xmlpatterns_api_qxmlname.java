import org.qtjambi.qt.gui.QApplication;


public class src_xmlpatterns_api_qxmlname {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
  <svg xmlns="http://www.w3.org/2000/svg"/>
//! [0]


//! [1]
  <x:svg xmlns:x="http://www.w3.org/2000/svg"/>
//! [1]


//! [2]
   // Fills the bits from beg to end with 1s and leaves the rest as 0.

   template<typename IntegralT>
    inline IntegralT bitmask(IntegralT begin, IntegralT end)
    {
        IntegralT filled_bits = (1 << (end - begin + 1)) - 1;
        return filled_bits << begin;
    }
//! [2]


    }
}
