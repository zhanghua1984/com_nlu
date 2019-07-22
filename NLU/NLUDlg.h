
// NLUDlg.h: 头文件
//

#pragma once

#include "comport.h"
#include "afxcmn.h"
#include "afxwin.h"

// CNLUDlg 对话框
class CNLUDlg : public CDialogEx
{
// 构造
public:
	CNLUDlg(CWnd* pParent = nullptr);	// 标准构造函数
	void Initialization();
	CFont font;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NLU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	afx_msg LRESULT OnReceiveAComPort(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadRXMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadTXMessage(WPARAM wParam, LPARAM lParam);
// 实现
protected:
	HICON m_hIcon;
	comport m_comport;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_com_num;
	afx_msg void OnBnClickedButtonCom();
	CButton m_bn_com;
	afx_msg void OnBnClickedButtonLoopTest();
	CRichEditCtrl m_richeditctrl_rx;
	CEdit m_edit_rx_num;
	CRichEditCtrl m_richeditctrl_tx;
	CEdit m_edit_wakeup;
	CRichEditCtrl m_richeditctrl_input;
	CRichEditCtrl m_richeditctrl_result;
	CRichEditCtrl m_richedit_rx_intent;
	CRichEditCtrl m_richeditctrl_rx_value;
	afx_msg void OnBnClickedButtonAllClear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedCheckHex();
	CButton m_button_check;
	CRichEditCtrl m_richedit_inputjson;
	afx_msg void OnBnClickedButtonJson();
	CEdit m_edit_json_send_time;
	afx_msg void OnBnClickedCheckJsonSendTimer();
	CButton m_button_set_timer;
	afx_msg void OnBnClickedCheckClearAll();
	CButton m_buttonTimeClearAll;
	CEdit m_edit_time_clear;
};
