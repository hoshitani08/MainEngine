#pragma once

#include "Object3d.h"

class ContactableObject : public Object3d
{
public:
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns></returns>
	static ContactableObject* Create(Model* model = nullptr);
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model);
};