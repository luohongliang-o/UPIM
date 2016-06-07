#ifndef UIRECENTLINK_H
#define UIRECENTLINK_H

namespace DuiLib
{
class CRecentLinkUI : public CListUI
{
public:
	CRecentLinkUI(CPaintManagerUI& paint_manager);
	~CRecentLinkUI();

private:
	CPaintManagerUI& paint_manager_;
};

} // DuiLib

#endif // UIRECENTLINK_H