from qsdn import Locale as IQLocale
from qsdn import NumericValidator as NumberValidator
from qsdn import LimitingNumericValidator as CryptoCurrencyValidator
import unittest
from decimal import Decimal as D
import decimal
from PyQt5.QtCore import *
from PyQt5.QtGui import *



class NumberOption(unittest.TestCase):
    def setUp(self):
        self.rc_locale = IQLocale()
        self.rc_locale.setNumberOptions( QLocale.NumberOptions(QLocale.RejectGroupSeparator) )
        self.oc_locale = IQLocale()
        self.oc_locale.setNumberOptions( QLocale.NumberOptions(QLocale.OmitGroupSeparator) )

    def test_noOption(self):
        self.assertFalse(self.rc_locale is self.oc_locale)
        default_locale = IQLocale()
        self.assertEqual(default_locale, IQLocale.system())
        self.assertFalse( default_locale.numberOptions() == QLocale.NumberOptions(QLocale.RejectGroupSeparator) )
        self.assertNotEqual(QLocale.OmitGroupSeparator, QLocale.RejectGroupSeparator)
        self.assertFalse( default_locale.numberOptions() & QLocale.NumberOptions(QLocale.RejectGroupSeparator) == QLocale.NumberOptions(QLocale.RejectGroupSeparator) )
        self.assertFalse( default_locale.numberOptions() & QLocale.NumberOptions(QLocale.OmitGroupSeparator) == QLocale.NumberOptions(QLocale.OmitGroupSeparator) )
        self.assertFalse( default_locale.numberOptions() & QLocale.RejectGroupSeparator == QLocale.RejectGroupSeparator )
        self.assertFalse( default_locale.numberOptions() & QLocale.OmitGroupSeparator == QLocale.OmitGroupSeparator )

    def test_RejCommaOption(self):
        self.assertFalse(self.rc_locale.numberOptions() == QLocale.OmitGroupSeparator )
        self.assertTrue(self.rc_locale.numberOptions() & QLocale.RejectGroupSeparator == QLocale.RejectGroupSeparator)    
        self.assertTrue(self.rc_locale.numberOptions() & QLocale.OmitGroupSeparator == QLocale.NumberOptions(0)) 
            
    def test_omit_group_separator_option(self):
        self.assertEqual(self.oc_locale.numberOptions() & QLocale.NumberOptions(QLocale.OmitGroupSeparator), QLocale.NumberOptions(QLocale.OmitGroupSeparator))
        self.assertNotEqual(self.oc_locale.numberOptions() & QLocale.RejectGroupSeparator, QLocale.RejectGroupSeparator)


class TestNumericValidator(unittest.TestCase):
	def setUp(self):
		self.us_locale = IQLocale("en_US")
		self.validator = NumberValidator()
		self.validator.setLocale( self.us_locale )
		self.validate_strings = [ 'QValidator.Invalid', 'QValidator.Intermediate', 'QValidator.Acceptable' ]

		
	def test_validator_0(self):
		(status, result_string, pos) = self.validator.validate("", 0)
		self.assertEqual( "QValidator.Intermediate", self.validate_strings[status], msg="Validate validate empty string #0" )
	
	def test_validator_1(self):
		(status, result_string, pos) = self.validator.validate("0.0034", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.003,4", result_string)
	def test_validator_2(self):
		(status, result_string, pos) = self.validator.validate("0.001,423", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.001,423", result_string)
	def test_validator_3(self):
		(status, result_string, pos) = self.validator.validate("0.003,412,3", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.003,412,3", result_string)
	def test_validator_123456789(self):
		(status, result_string, pos) = self.validator.validate("123456789", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("123,456,789", result_string)
	def test_validator_5(self):
		(status, result_string, pos) = self.validator.validate("0.013410", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.013,410", result_string)
	def test_validator_6(self):
		(status, result_string, pos) = self.validator.validate("0.123,", 0)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("0.123,", result_string)
	def test_validator_7(self):
		(status, result_string, pos) = self.validator.validate(".42", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.42", result_string)
	def test_validator_doubledot(self):
		(status, result_string, pos) = self.validator.validate("0123.12.3", 0)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])

class CryptoCurencyUnspacedValidator(unittest.TestCase):
	def setUp(self):
		self.validator = CryptoCurrencyValidator(8, 8, False)
		self.validator.setLocale( IQLocale("en_US") )
		self.validate_strings = [ 'QValidator.Invalid', 'QValidator.Intermediate', 'QValidator.Acceptable' ]

	def test_validate_00034(self):
		(status, result_string, pos) = self.validator.validate("0.0034", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.003,4", result_string)

	def test_validate_0001423(self):
		(status, result_string, pos) = self.validator.validate("0.001,423", 0)
		self.assertEqual(("QValidator.Acceptable", QValidator.Acceptable, "0.001,423"), 
			(self.validate_strings[status], status, result_string))
	def test_validate_00034123(self):
		(status, result_string, pos) = self.validator.validate("0.003,412,3", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.003,412,3", result_string)

	def test_validate_123456789(self):
		(status, result_string, pos) = self.validator.validate("123456789", 0)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("12,345,678", result_string)

	def test_validate_0013410(self):
		(status, result_string, pos) = self.validator.validate("0.013410", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.013,410", result_string)

	def test_validate_0123(self):
		(status, result_string, pos) = self.validator.validate("0.123,", 0)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("0.123,", result_string)

	def test_validate_042(self):
		(status, result_string, pos) = self.validator.validate(".42", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.42", result_string)

	def test_validate_0123123(self):
		(status, result_string, pos) = self.validator.validate("0123.12.3", 0)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])
		
	def test_validate_1(self):
		(status, result_string, pos) = self.validator.validate("1", 1)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("1", result_string)
		self.assertEqual(1, pos)

	def test_validate_17(self):
		(status, result_string, pos) = self.validator.validate("17", 2)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("17", result_string)
		self.assertEqual(2, pos)

	def test_validate_17(self):
		(status, result_string, pos) = self.validator.validate("17,", 3)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("17,", result_string)
		self.assertEqual(3, pos)

	def test_validate_177(self):
		(status, result_string, pos) = self.validator.validate("177", 3)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("177", result_string)
		self.assertEqual(3, pos)

	def test_validate_177(self):
		(status, result_string, pos) = self.validator.validate("177,", 4)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("177,", result_string)
		self.assertEqual(4, pos)

	def test_validate_1777(self):
		(status, result_string, pos) = self.validator.validate("1777", 4)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("1,777", result_string)
		self.assertEqual(5, pos)

	def test_validate_17772(self):
		(status, result_string, pos) = self.validator.validate("1,7772", 6)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("17,772", result_string)
		self.assertEqual(6, pos)

	def test_validate_177721(self):
		(status, result_string, pos) = self.validator.validate("17,7721", 7)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("177,721", result_string)
		self.assertEqual(7, pos)

	def test_validate_1777216(self):
		(status, result_string, pos) = self.validator.validate("177,7216", 8)
		self.assertEqual("1,777,216", result_string)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(9, pos)

	def test_validate_23456789(self):
		(status, result_string, pos) = self.validator.validate("23456789", 4)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("23,456,789", result_string)
		self.assertEqual(5, pos)

	def test_validate_0013410(self):
		(status, result_string, pos) = self.validator.validate("0.013410", 7)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.013,410", result_string)
		self.assertEqual(8, pos)

	def test_validate_0000000001(self):
		(status, result_string, pos) = self.validator.validate("0.000,000,001", 14)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("0.000,000,00", result_string)
		self.assertEqual(12, pos)

	def test_validate_0000000000001(self):
		(status, result_string, pos) = self.validator.validate("0.000,000,000,001", 18)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("0.000,000,00", result_string)
		self.assertEqual(12, pos)

	def test_validate_one_hundred_thousand_with_bad_comma(self):
		(status, result_string, pos) = self.validator.validate("10,0000", 5)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("100,000", result_string)
		self.assertEqual(5, pos)

	def test_validate_one_hundred_million_with_bad_commas(self):
		(status, result_string, pos) = self.validator.validate("10,000,0000", 11)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual("10,000,000", result_string)
		self.assertEqual(10, pos)
		
	def test_validate_one_hundred_millino_bitcoins(self):
		(status, result_string, pos) = self.validator.validate("100,000,000", 11)
		

	def test_validate_00034(self):
		(status, result_string, pos) = self.validator.validate("0.0034", 2)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.003,4", result_string)
		self.assertEqual(2, pos)

	def test_validate_1(self):
		(status, result_string, pos) = self.validator.validate("1", 1)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("1", result_string)
		self.assertEqual(1, pos)

	def test_validate_0(self):
		(status, result_string, pos) = self.validator.validate("0", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0", result_string)
		self.assertEqual(0, pos)

	def test_validate_23124(self):
		(status, result_string, pos) = self.validator.validate("23124", 3)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("23,124", result_string)
		self.assertEqual(4, pos)

	def test_validate_symbols(self):
		(status, result_string, pos) = self.validator.validate("%!@", 2)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])

	def test_validate_00032(self):
		(status, result_string, pos) = self.validator.validate("0.003,2", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual("0.003,2", result_string)
		self.assertEqual(0, pos)

	def test_validate_Hello(self):
		(status, result_string, pos) = self.validator.validate("Hello", 2)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])

		
class TestNumericFormating(unittest.TestCase):
    def setUp(self):
        self.us_locale = IQLocale("en_US")

    def test_decimal_american_english(self):  
        TESTINPUT=0
        EXPECTED=1            

        context = decimal.getcontext()
        context.prec = 8+7
        decimal.setcontext(context)
        tests = ( (D("43112279.75467"), "43,112,279.754,67"), (D("0.0101020204"), "0.010,102,020,4"),
            (D("0.00000001"), "0.000,000,01")
            )
        verbose = False
        for c in tests:
            self.assertEqual( str(c[1]), self.us_locale.toString(c[0]), msg="Test case %s" % c[1] )
            self.assertEqual( (c[0], True), self.us_locale.toDecimal(c[1], 10), msg="Test case US locale parsing %s" % c[1] )
        
    def test_hex(self):
        self.us_locale = IQLocale("en_US")
        TESTINPUT=0
        EXPECTED=1            
        (d, good) = self.us_locale.toDecimal("0xB,ADF,00D", 0)
        self.assertEqual( True, good, msg="0xB,ADF,00D #1" )
        self.assertEqual( D("195948557"), d, msg="0xB,ADF,00D #2" )
        new_string = '0xFF0'
        (value, ok) = self.us_locale.toDecimal(new_string, 0)
        self.assertEqual( new_string, '0xFF0', msg = 'numbers passed are not modified')
        a_string = str('      100')
        self.assertEqual( ok,    True,  msg = "hex numbers without commas parse successfully")
        self.assertEqual( value, 0xFF0, msg = "hex numbers without commas parse correctly")
        
    def test_nonsense(self):   
        self.us_locale = IQLocale("en_US")
        TESTINPUT=0
        EXPECTED=1            
        (value, ok) = self.us_locale.toDecimal( str("") )
        self.assertEqual( ok, False )
        (value, ok) = self.us_locale.toDecimal( str("The cat came back ") )
        self.assertEqual( ok, False )


    def test_spanish(self):
        # European way of writing numbers style
        tests = ( (D("43112279.75467"), "43.112.279,754.67"), (D("0.0101020204"), "0,010.102.020.4"),
            (D("0.00000001"), "0,000.000.01")
            )
        spanishlocale = IQLocale("es_ES")
        self.assertEqual(spanishlocale.name(), "es_ES")
        for c in tests:
            self.assertEqual( c[1], spanishlocale.toString(c[0]), msg="Test case spanish to String %s" % c[1] )
            self.assertEqual( (c[0], True), spanishlocale.toDecimal(c[1], 10), msg="Test case spanish parsing %s" % c[1] )
            
                        
    def test_c(self):
        # C style values for tests
        tests = ( (D("43112279.75467"), "43112279.75467"), (D("0.0101020204"), "0.0101020204"),
            (D("0.00000001"), "0.00000001")
            )                
        clocale = IQLocale.c()
        self.assertEqual(clocale.name(), "C")
        for c in tests:
            self.assertEqual( str(c[1]), clocale.toString(c[0]), msg="Test case %s" % c[1] )
            self.assertEqual( (c[0], True), clocale.toDecimal(c[1], 10), msg="Test case C parsing %s" % c[1] )
        IQLocale.setDefault(IQLocale.system())
        
    def test_egyptian(self):
        egyptian = IQLocale( QLocale( QLocale.Arabic, QLocale.Egypt ) )
        self.assertEqual( ('\u0663\u066b\u0661\u0664\u0661\u066c\u0666'), egyptian.toString( D('3.1416') ) , msg= "Egyptian PI 5 digits" )

    def test_otherbases(self):
        self.assertEqual( (D("0.3125"), True), self.us_locale.toDecimal("0x0.5", 0), msg="5/16 hex #1" ) # 5/16
        self.assertEqual( (D("0.3125"), True), self.us_locale.toDecimal("0.5", 16), msg="5/16 hex #2" ) # 5/16
        self.assertEqual( (D("1.25"), True), self.us_locale.toDecimal("01.2", 0), msg="1 1/4 octal #1" )  # 1 1/4
        self.assertEqual( (D("1.25"), True), self.us_locale.toDecimal("1.2", 8), msg="1 1/4 octal #2" )  # 1 1/4
        self.assertEqual( (D("1.2"), True), self.us_locale.toDecimal("01.2", 10), msg="1 1/5 decimal #1" )        # 1 1/5
        self.assertEqual((D("1.2"), True), self.us_locale.toDecimal("1.2", 0), msg="1 1/5 decimal #2" )        # 1 1/5
        self.assertEqual( (D("6.375"), True), self.us_locale.toDecimal("110.011", 2), msg="4+2+1/4+1/8 binary" )
    	

    def test_quantized(self):
        self.assertEqual( "12.00", self.us_locale.toString(D("12").quantize(D('0.01'))), msg="12 dollars" )
        self.assertEqual( "3,379.70", self.us_locale.toString(D("3379.7").quantize(D('0.01'))) , msg="rubbles" )
        self.assertEqual( "636.40", self.us_locale.toString(D("636.4").quantize(D('0.01'))), msg= "cny" )
        self.assertEqual( "67.56", self.us_locale.toString(D("67.56")), msg= "GBP" )
        self.assertEqual( "103.00", self.us_locale.toString(D("103").quantize(D('0.01'))), msg= "USD" )
        self.assertEqual( "1,000,000", self.us_locale.toString(D(10)**6), msg= "1 million" )
        context = decimal.getcontext()
        context.prec = 30
        decimal.setcontext(context)
        self.assertEqual( "0.333,33", self.us_locale.toString((D("1")/3).quantize(D('0.00001'))) , msg= "1/3" )
        self.assertEqual( "0.50", self.us_locale.toString(D("1").quantize(D('0.01'))/2), msg= "1/2" )
                
                                            
class TestSpacedCryptoCurrencyValidator(unittest.TestCase):
	def setUp(self):
		us_locale = IQLocale("en_US")
		spacing_validator = CryptoCurrencyValidator(8, 8, True)
		spacing_validator.setLocale( us_locale )
		self.validate_strings = [ 'QValidator.Invalid', 'QValidator.Intermediate', 'QValidator.Acceptable' ]
		self.validator = spacing_validator
    
	def test_cryptocurrency_validator_Hello(self):
		(status, result_string, pos) = self.validator.validate("Hello", 2)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])

	def test_cryptocurrency_validator_1(self):
		(status, result_string, pos) = self.validator.validate("1", 1)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("         1", 10), (result_string, pos))

	def test_cryptocurrency_validator_17(self):
		(status, result_string, pos) = self.validator.validate("         17", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("        17", 10), (result_string, pos))

	def test_cryptocurrency_validator_17(self):
		(status, result_string, pos) = self.validator.validate("        17,", 11)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual(("       17,", 10), (result_string, pos))

	def test_cryptocurrency_validator_177(self):
		(status, result_string, pos) = self.validator.validate("        177", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("       177", 10), (result_string, pos))

	def test_cryptocurrency_validator_177(self):
		(status, result_string, pos) = self.validator.validate("       177,", 11)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual(("      177,", 10), (result_string, pos))

	def test_cryptocurrency_validator_1777(self):
		(status, result_string, pos) = self.validator.validate("       1777", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("     1,777", 10), (result_string, pos))

	def test_cryptocurrency_validator_17772(self):
		(status, result_string, pos) = self.validator.validate("     1,7772", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("    17,772", 10), (result_string, pos))

	def test_cryptocurrency_validator_177721(self):
		(status, result_string, pos) = self.validator.validate("    17,7721", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("   177,721", 10), (result_string, pos))

	def test_cryptocurrency_validator_1777216(self):
		(status, result_string, pos) = self.validator.validate("   177,7216", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual((" 1,777,216", 10), (result_string, pos))

	def test_cryptocurrency_validator_123456789(self):
		(status, result_string, pos) = self.validator.validate("123456789", 4)
		self.assertEqual("QValidator.Intermediate", self.validate_strings[status])
		self.assertEqual(("12,345,678", 5), (result_string, pos))

	def test_cryptocurrency_validator_0013410(self):
		(status, result_string, pos) = self.validator.validate("0.013410", 7)
		self.assertEqual(("QValidator.Acceptable","         0.013,410", 17), 
			(self.validate_strings[status],result_string, pos))

	def test_cryptocurrency_validator_00034(self):
		(status, result_string, pos) = self.validator.validate("0.0034", 2)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("         0.003,4", 11), (result_string, pos))

	def test_cryptocurrency_validator_1(self):
		(status, result_string, pos) = self.validator.validate("1", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("         1", 0), (result_string, pos))

	def test_cryptocurrency_validator_0(self):
		(status, result_string, pos) = self.validator.validate("0", 1)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("         0", 10), (result_string, pos))

	def test_cryptocurrency_validator_23124(self):
		(status, result_string, pos) = self.validator.validate("23124", 3)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("    23,124", 8), (result_string, pos))

	def test_cryptocurrency_validator_symbol(self):
		(status, result_string, pos) = self.validator.validate("%!@", 2)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])

	def test_cryptocurrency_validator_0471400(self):
		(status, result_string, pos) = self.validator.validate("        0.471,400", 8)
		self.assertEqual(("QValidator.Acceptable","         0.471,400", 9), (self.validate_strings[status],result_string, pos))

	def test_cryptocurrency_validator_112134210001(self):
		(status, result_string, pos) = self.validator.validate("11,213,421.000,1", 0)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("11,213,421.000,1", 0), (result_string, pos))

	def test_cryptocurrency_validator_100(self):
		(status, result_string, pos) = self.validator.validate("        100", 11)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual(("       100", 10), (result_string, pos))

	def test_cryptocurrency_validator_1000000(self):
		(status, result_string, pos) = self.validator.validate(" 1,000,000", 9)
		self.assertEqual("QValidator.Acceptable", self.validate_strings[status])
		self.assertEqual((" 1,000,000", 9), (result_string, pos))

	def test_cryptocurrency_validator_Hello(self):
		(status, result_string, pos) = self.validator.validate("Hello", 2)
		self.assertEqual("QValidator.Invalid", self.validate_strings[status])	


class TestParsing(unittest.TestCase):
    def test_all(self):
        locale = IQLocale('US')
        locale.toShort('321')
        self.assertEqual( (321, True) , locale.toShort('321', 10), msg = "tiny value to short")
        self.assertEqual( False, locale.toShort('70,000', 10)[1], msg = "value to to short is too long")
        self.assertEqual( (32000, True), locale.toShort('32,000', 10), msg = "big value to short")
        self.assertEqual( (60000, True), locale.toUShort('60,000', 10), msg = "big value to ushort")
        self.assertEqual( (2000000, True), locale.toInt('2,000,000', 10), msg = "big int value to int")
        self.assertEqual( (4000000, True), locale.toUInt('4,000,000', 10), msg = "big uint value to int")
        locale = IQLocale.c()
        self.assertEqual( (321, True) , locale.toShort('321', 10), msg = "tiny value to short")
        self.assertEqual( False, locale.toShort('70000', 10)[1], msg = "value to short is too long")
        self.assertEqual( (32000, True), locale.toShort('32000', 10), msg = "big value to short")
        self.assertEqual( (60000, True), locale.toUShort('60000', 10), msg = "big value to ushort")
        self.assertEqual( (2000000, True), locale.toInt('2000000', 10), msg = "big int value to int")
        self.assertEqual( (4000000, True), locale.toUInt('4000000', 10), msg = "big uint value to int")
        
class TestStatics(unittest.TestCase):
	def setUp(self):
		pass
	
	def test_system(self):
		self.assertEqual(QLocale.system().name(), IQLocale.system().name(), "IQLocale and QLocale .system return locales with the same name")
	
	def test_system_country(self):
		self.assertEqual(QLocale.system().country(), IQLocale.system().country(), "IQLocale and QLocale .system return locales with the same country")

	def test_system_lang(self):
		self.assertEqual(QLocale.system().language(), IQLocale.system().language(), "IQLocale and QLocale .system return locales with the same language")

	def test_c(self):
		self.assertEqual(QLocale.c().name(), IQLocale.c().name(), "IQLocale and QLocale .c  return locales with the same name")

	def test_c_country(self):
		self.assertEqual(QLocale.c().country(), IQLocale.c().country(), "IQLocale and QLocale .c  return locales with the same country")

	def test_c_lang(self):
		self.assertEqual(QLocale.c().language(), IQLocale.c().language(), "IQLocale and QLocale .c  return locales with the same language")

	def test_default(self):
		self.assertEqual(QLocale().name(), IQLocale().name(), "IQLocale and QLocale default return locales with the same name")

	def test_default_country(self):
		self.assertEqual(QLocale().country(), IQLocale().country(), "IQLocale and QLocale default return locales with the same country")
       
       
if __name__ == '__main__':
    unittest.main()


