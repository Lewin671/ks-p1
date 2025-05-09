#include "Command.h"

// 目前所有 Command 的实现都在 Command.h 中作为内联或模板提供
// 如果有非内联的 Command 实现，可以放在这里。
// 例如，如果 ICommand 有非纯虚的析构函数，其定义应在此处。
// ICommand::~ICommand() = default; // 如果在 .h 中声明了但未定义，可以在此定义