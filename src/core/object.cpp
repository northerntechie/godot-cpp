/**************************************************************************/
/*  object.cpp                                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include <godot_cpp/core/object.hpp>

#include <godot_cpp/core/class_db.hpp>

namespace godot {

namespace internal {

Object *get_object_instance_binding(GodotObject *p_engine_object) {
	if (p_engine_object == nullptr) {
		return nullptr;
	}

	// Get existing instance binding, if one already exists.
	GDExtensionObjectPtr instance = gdextension_interface_object_get_instance_binding(p_engine_object, token, nullptr);
	if (instance != nullptr) {
		return reinterpret_cast<Object *>(instance);
	}

	// Otherwise, try to look up the correct binding callbacks.
	const GDExtensionInstanceBindingCallbacks *binding_callbacks = nullptr;
	StringName class_name;
	if (gdextension_interface_object_get_class_name(p_engine_object, library, reinterpret_cast<GDExtensionStringNamePtr>(class_name._native_ptr()))) {
		binding_callbacks = ClassDB::get_instance_binding_callbacks(class_name);
	}
	if (binding_callbacks == nullptr) {
		binding_callbacks = &Object::___binding_callbacks;
	}

	return reinterpret_cast<Object *>(gdextension_interface_object_get_instance_binding(p_engine_object, token, binding_callbacks));
}

} // namespace internal

MethodInfo::MethodInfo() :
		flags(GDEXTENSION_METHOD_FLAG_NORMAL) {}

MethodInfo::MethodInfo(StringName p_name) :
		name(p_name), flags(GDEXTENSION_METHOD_FLAG_NORMAL) {}

MethodInfo::MethodInfo(Variant::Type ret) :
		flags(GDEXTENSION_METHOD_FLAG_NORMAL) {
	return_val.type = ret;
}

MethodInfo::MethodInfo(Variant::Type ret, StringName p_name) :
		name(p_name), flags(GDEXTENSION_METHOD_FLAG_NORMAL) {
	return_val.type = ret;
}

MethodInfo::MethodInfo(const PropertyInfo &p_ret, StringName p_name) :
		name(p_name), return_val(p_ret), flags(GDEXTENSION_METHOD_FLAG_NORMAL) {}

} // namespace godot
