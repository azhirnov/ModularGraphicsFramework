// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "ScriptBinder.h"

#ifdef GX_ANGELSCRIPT_ENABLED

namespace GX_STL
{
namespace GXScript
{

namespace _script_hidden_
{
/*
=================================================
	_UnaryToStr
=================================================
*/
	const char *  OperatorBinderHelper::_UnaryToStr (EUnaryOperator op)
	{
		static const char *	names[] = {
			"opNeg",		// OP_INVERSE
			"opCom",		// OP_INVERSE
			"opPreInc",		// OP_PRE_INC
			"opPreDec",		// OP_PRE_DEC
			"opPostInc",	// OP_POST_INC
			"opPostDec"		// OP_POST_DEC
		};
		return names[ op ];
	}
	
/*
=================================================
	_BinToStr
=================================================
*/
	const char *  OperatorBinderHelper::_BinToStr (EBinaryOperator op)
	{
		static const char *	names[] = {
			"opAdd",		// OP_ADD
			"opSub",		// OP_SUB
			"opMul",		// OP_MUL
			"opDiv",		// OP_DIV
			"opMod",		// OP_MOD
			// opPow
			"opAnd",		// OP_AND
			"opOr",			// OP_OR
			"opXor",		// OP_XOR
			"opShl",		// OP_SHIFT_LEFT
			"opShr",		// OP_SHIFT_RIGHT
			// opUShr 
		};
		return names[ op ];
	}
	
/*
=================================================
	_BinAssignToStr
=================================================
*/
	const char *  OperatorBinderHelper::_BinAssignToStr (EBinaryOperator op)
	{
		static const char *	names[] = {
			"opAddAssign",		// OP_ADD
			"opSubAssign",		// OP_SUB
			"opMulAssign",		// OP_MUL
			"opDivAssign",		// OP_DIV
			"opModAssign",		// OP_MOD
			// opPowAssign
			"opAndAssign",		// OP_AND
			"opOrAssign",		// OP_OR
			"opXorAssign",		// OP_XOR
			"opShlAssign",		// OP_SHIFT_LEFT
			"opShrAssign"		// OP_SHIFT_RIGHT
			// opUShrAssign 
		};
		return names[ op ];
	}
	
/*
=================================================
	_BinRightToStr
=================================================
*/
	const char *  OperatorBinderHelper::_BinRightToStr (EBinaryOperator op)
	{
		static const char *	names[] = {
			"opAdd_r",		// OP_ADD
			"opSub_r",		// OP_SUB
			"opMul_r",		// OP_MUL
			"opDiv_r",		// OP_DIV
			"opMod_r",		// OP_MOD
			// opPow_r
			"opAnd_r",		// OP_AND
			"opOr_r",		// OP_OR
			"opXor_r",		// OP_XOR
			"opShl_r",		// OP_SHIFT_LEFT
			"opShr_r",		// OP_SHIFT_RIGHT
			//opUShr_r 
		};
		return names[ op ];
	}

}	// _script_hidden_

}	// GXScript
}	// GX_STL

#endif	// GX_ANGELSCRIPT_ENABLED
