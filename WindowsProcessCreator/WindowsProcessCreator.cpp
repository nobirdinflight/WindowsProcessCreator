#include "WindowsProcessCreator.h"
#include "ProcessWrapper.h"
WindowsProcessCreator::WindowsProcessCreator(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.choose_button, &QPushButton::clicked, this, &WindowsProcessCreator::choose);
	connect(ui.launch_button, &QPushButton::clicked, this, &WindowsProcessCreator::launch);
	connect(ui.apply_base_settings_button, &QPushButton::clicked, this, &WindowsProcessCreator::applyBaseSettings);
#ifdef _DEBUG
	file = LR"(C:\Windows\notepad.exe)";
	ui.path_edit->setText(QString::fromStdWString(file));
#endif

	setProcessesCombo();
}

WindowsProcessCreator::~WindowsProcessCreator()
{}

void WindowsProcessCreator::choose()
{
	auto file_qstring = QFileDialog::getOpenFileName(this, "Please choose executable file.", R"(C:\Windows)", "File (*.exe)");
	ui.path_edit->setText(file_qstring);
	file = file_qstring.toStdWString();
}

template <typename T,
	typename TIter = decltype(std::begin(std::declval<T>())),
	typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T&& iterable) {
	struct iterator {
		int i;
		TIter iter;
		bool operator != (const iterator& other) const { return iter != other.iter; }
		void operator ++ () { ++i; ++iter; }
		auto operator * () const { return std::tie(i, *iter); }
	};
	struct iterable_wrapper {
		T iterable;
		auto begin() { return iterator{ 0, std::begin(iterable) }; }
		auto end() { return iterator{ 0, std::end(iterable) }; }
	};

	return iterable_wrapper{ std::forward<T>(iterable) };
}

void WindowsProcessCreator::setProcessesCombo()
{
	std::vector<size_t> pids{};
	enumeratePIDs(pids);
	std::sort(pids.begin(), pids.end());
	for (auto [index, pid] : enumerate(pids))
	{
		std::wstring path;
		if (getProcessImagePathFromPID(pid, path) == ERROR_SUCCESS && !path.empty())
		{
			ui.select_parent_combo->insertItem(index, QString::fromStdWString(L"PID " + std::to_wstring(pid) + L": " + path));
		}
	}
}

void WindowsProcessCreator::launch()
{
	if (enable_base_settings)
	{
		createProcessWithSettings(file, parent_pid);
	}
	else
	{
		createProcessDefault(file);
	}
}

void WindowsProcessCreator::applyBaseSettings()
{
	enable_base_settings = true;
	auto text = ui.select_parent_combo->currentText().toStdWString();
	auto extractPID = [](const std::wstring& text)->size_t
	{
		constexpr size_t begin_index = 4;
		auto end_index = text.find(L':');
		if (end_index == std::wstring::npos)
		{
			return 0;
		}
		return std::stoull(text.substr(begin_index, end_index - begin_index));
	};
	parent_pid = extractPID(text);
}