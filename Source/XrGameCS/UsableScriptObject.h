#pragma once

class CGameObject;

class CUsableScriptObject
{
public:
							CUsableScriptObject		();
							~CUsableScriptObject	();
		virtual bool		use						(CGameObject* who_use);
	
		//������� ������������ ��� ��������� �� ������ (���� NULL, �� ���)
		virtual LPCSTR		tip_text				();
		void				set_tip_text			(LPCSTR new_text);
		virtual void		set_tip_text_default	();

		//����� �� ������������ ������ ����������� (�� ����������) �������
		bool				nonscript_usable		();
		void				set_nonscript_usable	(bool usable);
private:
		shared_str			m_sTipText;
		bool				m_bNonscriptUsable;
};
