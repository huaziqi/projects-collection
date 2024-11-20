import PySimpleGUI as sg
import time
import csv
import os

QT_ENTER_KEY1 = 'special 16777220'
QT_ENTER_KEY2 = 'special 16777221'

floder = "data"

def create_file(the_name):
    file = open("data/" + the_name, "w", encoding = "utf-8")
    file.close()
def read_file(the_name, re_name):
    re_name = open("data/", "r", encoding = "utf-8")
def write_file(the_name, wr_name):
    wr_name = open("data/", "r", encoding = "utf-8")

if(not os.path.exists(floder)):
    os.makedirs(floder)
if(not os.path.exists("data/done.txt")):
    create_file("done.txt")
if(not os.path.exists("data/basic.txt")):
    create_file("basic.txt")
if(not os.path.exists("data/task.txt")):
    create_file("task.txt")


def main():
    task_able = 0 #
    exp_num = 0 #
    exp_mod = 0 #
    now_exp = 0 #
    done = False #
    level = 0 #
    task = [] # 任务存储
    list_value = []
    list_valuere = []
    name = ""


#################################### 初始界面
    sg.theme('LightBrown4')
    

    # with open("data/basic.txt" , "r", encoding = "utf-8") as f1:
    #     message = f1.read()
    #     f1.close()

    # with open("data/done.txt", "w", encoding = "utf-8") as f2:
    #     f2.write("1")
    #     f2.close()

    with open("data/done.txt", "r", encoding = "utf-8") as f1:
        message = f1.read()
        if(len(message) == 1 and message[0] == "1"):
            done = True
        elif(len(message) > 1):
            done = False
            sg.popup("请勿更改文件夹数据！")
            


    if(done == False):       
        pp_lay = [
        [sg.Text("输入用户名", font = ("华文隶书", 18)), sg.InputText(font = ("兰亭黑-简", 15))],
        [sg.Text("自定义升级经验值", font = ("华文隶书", 18)), sg.InputText(font = ("兰亭黑-简", 15), do_not_clear = False)],
        [sg.OK(size = (100, 0))]
        ]
        pp_window = sg.Window("login", pp_lay)
        while True:
            pp_event, pp_value = pp_window.read()
            if pp_event == sg.WIN_CLOSED:
                return
            if(pp_event == "OK"):
                name = pp_value[0]
                exmod = 0
                is_num = True
                for i in range(0, len(pp_value[1])):
                    if(pp_value[1][i] >= '0' and pp_value[1][i] <= '9'):
                        exmod = int(pp_value[1][i]) + 10 * exmod
                    else:
                        is_num = False
                        break
                if(is_num == False):
                    sg.popup("经验值必须为数字")
                elif exmod == 0:
                    sg.popup("经验值需大于0")
                else:
                    exp_mod = exmod
                    done = True
                    with open("data/done.txt", "w", encoding = "utf-8") as f1:
                        f1.write("1")
                        f1.close()
                    with open("data/basic.txt", "w", encoding = "utf-8") as f1:
                        f1.write(name + '\n')
                        f1.write("1" + str(exp_mod) + '\n') # exp_mod
                        f1.write("2" + str(0) + '\n') # level
                        f1.write("3" + str(0) + '\n') # now_exp
                        f1.write("4" + str(0) + '\n') # exp_num
                        f1.write("5" + str(0) + '\n') # task_able
                        f1.close()
                    break
        pp_window.close()
    else:
        with open("data/basic.txt", "r", encoding = "utf-8") as f1:
            message = f1.readlines()
            name = message[0][0:len(message[0]) - 1]
            exp_mod = int(message[1][1:len(message[1]) - 1])
            level = int(message[2][1:len(message[2]) - 1])
            now_exp = int(message[3][1:len(message[3]) - 1])
            exp_num = int(message[4][1:len(message[4]) - 1])
            task_able = int(message[5][1:len(message[5]) - 1])
            f1.close()
        with open("data/task.txt", "r", encoding = "utf-8") as f1:
            message = f1.readlines()
            for i in range(0, len(message) // 4):
                for j in range(0, 4):
                    if(j == 0):
                        a1 = message[i * 4 + j]
                    elif(j == 1):
                        a2 = message[i * 4 + j]
                    elif(j == 2):
                        a3 = message[i * 4 + j]
                    elif(j == 3):
                        a4 = message[i * 4 + j]
                task.append([a1[0:len(task) - 1], int(a2[0:len(task) - 1]), a3[0:len(task) - 1], int(a4[0])])
    init_lay = [
            [sg.Text('明日复明日，明日何其多。', font = ("华文行楷", 30), size = (45, 1), justification = 'center')],
            [sg.Text("hi, " + name + " !  " + "level: " + str(level), font = ("华文隶书", 23)), sg.Text("Exp:", font = ("华文隶书", 20)), sg.ProgressBar(exp_mod, orientation = 'h', key = "progress_bar", size = (20, 20)), sg.Text(" " + str(now_exp) + "/" + str(exp_mod), font = ("华文隶书", 23))],
            [sg.Text("现有任务：", font = ("华文隶书", 20), size = (20, 1), justification = 'left'), sg.Text("交互窗口",  font = ("华文隶书", 20), size = (30, 0), justification = 'right')],
            ]
    list_value = []
    list_valuere = []
    for i in range(0, len(task)):
        if(task[i][3] != 2):
            list_value.append(str(i) + ". " + task[i][0])
            list_valuere.append(str(i) + ". " + task[i][0])
    list_valuere.reverse()
    init_lay += [sg.Listbox(values = (list_valuere), enable_events = True,size = (30, min(10, 1 * task_able)), font = ("黑体-简", 20)), sg.Output(size = (30 , 10), font = ("兰亭黑-简", 15))],
    init_lay += [sg.Button('添加任务', font = ("华文隶书", 15)), sg.Button('cmd', font = ("华文隶书", 15)), sg.Text("<--更多命令", font = ("华文隶书", 17))],
    init_window = sg.Window('打卡levelup_1.0.0', init_lay, size = (800, 400), finalize = True)
    progressbar = init_window['progress_bar']
    progressbar.UpdateBar(now_exp)
#################################### 


    while True:

        init_event, init_values = init_window.read()
        # print(init_event)
        if init_event == sg.WIN_CLOSED or init_event == 'Exit':
            with open("data/basic.txt", "w", encoding = "utf-8") as f1:
                message = ""
                message += name + '\n' + "1" + str(exp_mod) + '\n' + "2" + str(level) + '\n'
                message += "3" + str(now_exp) + '\n' + "4" + str(exp_num) + '\n' + "5" + str(task_able) + "\n"
                f1.write(message)
                f1.close()
            message = ""
            with open("data/task.txt", "w", encoding = "utf-8") as f1:
                for i in range(0, len(task)):
                    message += task[i][0] + "\n" + str(task[i][1]) + "\n"
                    message += task[i][2] + "\n" + str(task[i][3]) + "\n"
                f1.write(message)
            break

#################################### cmd
        if(init_event == 'cmd'):
            cmd_lay = [[sg.Text('现有指令', font = ("华文隶书", 20))],
            [sg.Text('- over 任务编号 || 例如：over 0 || 作用：完成任务，获取经验', font = ("兰亭黑-简", 15))],
            [sg.Text('- del 任务编号 || 例如：del 0 || 作用：删除任务', font = ("兰亭黑-简", 15))],
          [sg.InputText(key='-IN-', font = ("兰亭黑-简", 15), do_not_clear = False)]]
            cmd_window = sg.Window('控制台cmd', cmd_lay,
                   return_keyboard_events=True,
                   location = (200, 200)
                   )
            while True:             # Event Loop
                cmd_event, cmd_values = cmd_window.read()
                if cmd_event == sg.WIN_CLOSED:
                    break
                if cmd_event in ('\r', QT_ENTER_KEY1, QT_ENTER_KEY2):         # Check for ENTER key
                    # go find element with Focus
                    print("$ "+ cmd_values['-IN-'])
                    cmd_n = cmd_values['-IN-']
                    elem = cmd_window.find_element_with_focus()
#################################### 删除任务
                    if(cmd_n[0 : 3] == "del"):
                        if(len(task) == 0):
                            print("Error:无任务")
                        else:
                            if(cmd_n[3] != ' '):
                                print("Error:无空格")
                            else:
                                cmd_del_num = 0
                                cmd_del_isnum = True
                                for i in range(4, len(cmd_n)):
                                    
                                    if(cmd_n[i] >= '0' and cmd_n[i] <= '9'):
                                        cmd_del_num = cmd_del_num * 10 + int(cmd_n[i])
                                    else:
                                        cmd_del_isnum = False
                                        break
                                if(cmd_del_isnum == True):
                                    if(cmd_del_num >= 0 and cmd_del_num < len(task)):
                                        if(task[cmd_del_num][3] == 2):
                                            print("Error:已删除")
                                        else:
                                            task[cmd_del_num][3] = 2
                                            init_window.close()
                                            cmd_window.close()
                                            init_lay = [
                                            [sg.Text('明日复明日，明日何其多。', font = ("华文行楷", 30), size = (45, 1), justification = 'center')],
                                            [sg.Text("hi, " + name + " !  " + "level: " + str(level), font = ("华文隶书", 23)), sg.Text("Exp:", font = ("华文隶书", 20)), sg.ProgressBar(exp_mod, orientation = 'h', key = "progress_bar", size = (20, 20)), sg.Text(" " + str(now_exp) + "/" + str(exp_mod), font = ("华文隶书", 23))],
                                            [sg.Text("现有任务：", font = ("华文隶书", 20), size = (20, 1), justification = 'left')],
                                ]
                                            list_value = []
                                            list_valuere = []
                                            task_able -= 1
                                            for i in range(0, len(task)):
                                                if(task[i][3] !=  2):
                                                    list_value.append(str(i) + ". " + task[i][0])
                                                    list_valuere.append(str(i) + ". " + task[i][0])
                                            
                                            list_valuere.reverse()

                                            init_lay += [sg.Listbox(values = (list_valuere), enable_events = True, size = (30, min(10, 1 * task_able)), font = ("黑体-简", 20)), sg.Output(size = (30 , 10), font = ("兰亭黑-简", 15))],
                                            init_lay += [sg.Button('添加任务', font = ("华文隶书", 15)), sg.Button('cmd', font = ("华文隶书", 15)), sg.Text("<--更多命令", font = ("华文隶书", 17))],
                                            
                                            
                                            init_window = sg.Window('打卡levelup_1.0.0', init_lay, size = (800, 400), finalize = True)
                                            
                                            progressbar = init_window['progress_bar']
                                            
                                            progressbar.UpdateBar(now_exp)
                                            print("删除成功")

                                    else:
                                        print("Error:找不到任务")


                                else:
                                    print("Error:数字输入")
#################################### 完成任务
                    elif(cmd_n[0:4] == "over"):
                        if(len(task) == 0):
                            print("Error:无任务")
                        else:
                            if(cmd_n[4] != ' '):
                                print("Error:无空格")
                            else:
                                cmd_over_num = 0
                                cmd_over_isnum = True
                                for i in range(5, len(cmd_n)):
                                    print(cmd_n[i])

                                    if(cmd_n[i] >= '0' and cmd_n[i] <= '9'):
                                        cmd_over_num = cmd_over_num * 10 + int(cmd_n[i])
                                    else:
                                        cmd_over_isnum = False
                                        break
                                if(cmd_over_isnum == True):
                                    if(cmd_over_num >= 0 and cmd_over_num < len(task)):
                                        if(task[cmd_over_num][3] == 2):
                                            print("Error:任务已删除")
                                        elif(task[cmd_over_num][3] == 3):
                                            print("Error:任务已完成")
                                        else:
                                            task[cmd_over_num][3] = 3
                                            init_window.close()
                                            cmd_window.close()

                                            
                                            exp_num += task[cmd_over_num][1]
                                            level = exp_num // exp_mod
                                            now_exp = exp_num % exp_mod
                                            
                                            init_lay = [
                                            [sg.Text('明日复明日，明日何其多。', font = ("华文行楷", 30), size = (45, 1), justification = 'center')],
                                            [sg.Text("hi, " + name + " !  " + "level: " + str(level), font = ("华文隶书", 23)), sg.Text("Exp:", font = ("华文隶书", 20)), sg.ProgressBar(exp_mod, orientation = 'h', key = "progress_bar", size = (20, 20)), sg.Text(" " + str(now_exp) + "/" + str(exp_mod), font = ("华文隶书", 23))],
                                            [sg.Text("现有任务：", font = ("华文隶书", 20), size = (20, 1), justification = 'left')],
                                ]
                                            list_value = []
                                            list_valuere = []


                                            for i in range(0, len(task)):
                                                if(task[i][3] !=  2):
                                                    list_value.append(str(i) + ". " + task[i][0])
                                                    list_valuere.append(str(i) + ". " + task[i][0])
                                            list_valuere.reverse()
                                            init_lay += [sg.Listbox(values = (list_valuere), enable_events = True, size = (30, min(10, 1 * task_able)), font = ("黑体-简", 20)), sg.Output(size = (30 , 10), font = ("兰亭黑-简", 15))],
                                            init_lay += [sg.Button('添加任务', font = ("华文隶书", 15)), sg.Button('cmd', font = ("华文隶书", 15)), sg.Text("<--更多命令", font = ("华文隶书", 17))],
                                            
                                            init_window = sg.Window('打卡levelup_1.0.0', init_lay, size = (800, 400), finalize = True)
                                            progressbar = init_window['progress_bar']
                                            progressbar.UpdateBar(now_exp)


                                            print("任务已完成")
                                            
                    else:
                        print("Error:找不到指令")

            cmd_window.close()

#################################### 输出任务内容
        if(len(init_values) != 0 and len(init_values[0]) != 0):
            if(init_values[0][0] in list_value):
                position = list_value.index(init_values[0][0])
                print("$ ")
                print("任务名称: "+ task[position][0], " Exp:", task[position][1])
                print("描述：")
                print(task[position][2])
                if(task[position][3] == 3):
                    print("状态：已完成")
                else:
                    print("状态：未完成")
                print()
#################################### 日志



#################################### 添加任务
        if(init_event == '添加任务'):
            if(True):
                init_window.close()
                add_lay = [
                    [sg.Text('添加打卡任务', font = ("华文隶书", 25))],
                    [sg.Text('任务:',font = ("华文隶书", 18)), sg.InputText(font = ("兰亭黑-简", 15))],
                    [sg.Text('exp :  ',font = ("华文隶书", 18)), sg.InputText(font = ("兰亭黑-简", 15), do_not_clear=False)],
                    [sg.Text('任务描述', font = ("华文隶书", 18))],
                    [sg.Multiline(font = ("兰亭黑-简", 15),  size = (70, 5))],

                    [sg.Button('添加', font = ("华文隶书", 15)), sg.Button('返回', font = ("华文隶书", 15), key = 'back')]
    ]
                add_window = sg.Window('添加', add_lay, finalize = True)# add_window为添加任务界面

                while True:
                    add_event, add_values = add_window.read()
                    
                    if add_event == sg.WIN_CLOSED or add_event == 'Exit':
                        break
                    if add_event == '添加' and add_values[0] != '' and add_values[1] != '':
                        
                        num_flag = True;
                        for j in add_values[1]:
                            if(j < '0' or j > '9'):
                                num_flag = False
                        if(num_flag):
                            sg.popup("添加成功")
                            task.append([add_values[0], int(add_values[1]), add_values[2], 1])

                            task_able += 1
                        else:
                            sg.popup('exp必须为数字！', font = ("华文隶书", 20))
                            

                    elif (add_values[0] == '' or add_values[1] == '') and add_event == '添加':
                        sg.popup('任务或经验不能为空！', font = ("华文隶书", 20))
                    if(add_event == 'back'):
    #################################### 重构init_window
                        init_lay = [
                            [sg.Text('明日复明日，明日何其多。', font = ("华文行楷", 30), size = (50, 1), justification = 'center')],
                            [sg.Text("hi, " + name + " !  " + "level: " + str(level), font = ("华文隶书", 23)), sg.Text("Exp:", font = ("华文隶书", 20)), sg.ProgressBar(exp_mod, orientation = 'h', key = "progress_bar", size = (20, 20)), sg.Text(" " + str(now_exp) + "/" + str(exp_mod), font = ("华文隶书", 23))],
                            [sg.Text("现有任务：", font = ("华文隶书", 20), size = (15, 1), justification = 'left')]
        ]
                        list_value = []
                        list_valuere = []
                        for i in range(0, len(task)):
                            if(task[i][3] != 2):
                                list_value.append(str(i) + ". " + task[i][0])
                                list_valuere.append(str(i) + ". " + task[i][0])
                        
                        list_valuere.reverse()
                        init_lay += [
                        [sg.Listbox(values = (list_valuere),enable_events = True, size = (30, min(10, 1 * task_able)), font = ("黑体-简", 20)), sg.Output(size = (30, 10), font = ("兰亭黑-简", 15))],
                                            [sg.Button('添加任务', font = ("华文隶书", 15)), sg.Button('cmd', font = ("华文隶书", 15)), sg.Text("<--更多命令", font = ("华文隶书", 17))]
                                            ]

                        init_window = sg.Window('打卡levelup_1.0.0', init_lay, size = (800, 400), finalize = True)
                        progressbar = init_window['progress_bar']
                        progressbar.UpdateBar(now_exp)
                        
 
                        add_window.close();
#################################### 
                    
    init_window.close()


if __name__ == '__main__':
    main()


