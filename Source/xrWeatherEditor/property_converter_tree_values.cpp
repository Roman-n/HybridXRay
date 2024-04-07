////////////////////////////////////////////////////////////////////////////
//	Module 		: property_converter_tree_values.cpp
//	Created 	: 21.12.2007
//  Modified 	: 21.12.2007
//	Author		: Dmitriy Iassenev
//	Description : property converter color class
////////////////////////////////////////////////////////////////////////////

#include "pch.hpp"
#include "property_converter_tree_values.hpp"
#include "property_color.hpp"

using Flobbster::Windows::Forms::PropertyBag;
using System::ArgumentException;
using System::Attribute;
using System::Object;
using System::String;
using System::ComponentModel::PropertyDescriptor;
using System::ComponentModel::PropertyDescriptorCollection;
using System::ComponentModel::TypeDescriptor;

typedef PropertyBag::PropertySpecDescriptor PropertySpecDescriptor;

bool                                        property_converter_tree_values::CanConvertFrom(ITypeDescriptorContext ^ context, Type ^ source_type)
{
    return (false);
}
