import lldb


def print_matrix(
    debugger: lldb.SBDebugger,
    command: str,
    result: lldb.SBCommandReturnObject,
    internal_dict: dict,
):
    r"""
    This function is used to print Eigen::Matrix
    LLDB Script is just like this:
    ```
    script import importlib
    script import lldb_script.print_matrix as print_matrix
    command script add -f print_matrix.print_matrix pmat
    ```
    And then you can use
    `pmat <matrix>`
    to print Eigen matrix pretty
    """
    target: lldb.SBTarget = debugger.GetSelectedTarget()
    interpreter: lldb.SBCommandInterpreter = debugger.GetCommandInterpreter()
    process: lldb.SBProcess = target.GetProcess()
    thread: lldb.SBThread = process.GetSelectedThread()
    frame: lldb.SBFrame = thread.GetSelectedFrame()
    matrix_ptr: lldb.SBValue = frame.EvaluateExpression(f"{command}.m_storage.m_data")
    matrix_row: lldb.SBValue = frame.EvaluateExpression(f"{command}.m_storage.m_rows")
    matrix_col: lldb.SBValue = frame.EvaluateExpression(f"{command}.m_storage.m_cols")
    ptr_type: lldb.SBType = matrix_ptr.GetType()
    ptr_to_type_string: str = ptr_type.GetName()
    message: lldb.SBCommandReturnObject = lldb.SBCommandReturnObject()
    command_line = (
        f"parray {matrix_row.GetValue()} "
        f"reinterpret_cast<"
        f"{ptr_to_type_string.split(' ')[0]}(*)[{matrix_col.GetValue()}]"
        f">({command}.m_storage.m_data)"
    )
    interpreter.HandleCommand(command_line, message)
    if message.Succeeded():
        print(message.GetOutput(), end="")
    else:
        raise RuntimeError(message.GetError())
