from flask import Flask
from flask import request, render_template, abort, redirect, url_for, session
from datetime import datetime, timedelta
from subprocess import Popen, PIPE, STDOUT
import fcntl, os, json, re
from werkzeug.contrib.fixers import ProxyFix

app = Flask(__name__)

# set it to a random string
app.secret_key = 'fuckyouaaaaaaaa'

# set this to path/to/your/database/backend/program
database_exec_path = './code'

def app_init():
    app.proc = Popen([database_exec_path], stdin=PIPE, stdout=PIPE)
    fd = app.proc.stdout.fileno()
    fl = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)

app_init()

def db_write(cmd):
    cmdlist = re.split(r'[ ]', cmd)
    for cmdb in cmdlist:
        app.proc.stdin.write((cmdb + '\n').encode())
        app.proc.stdin.flush()

def db_readline():
    res = ""
    line = ""
    while True:
        try:
            line = app.proc.stdout.read().decode('utf-8')
            if line == "" or line == "\n":
                return res
            else:
                res += line
        except:
            break
    return res

def db_communicate(cmd):
    line = ""
    res = ""
    if cmd == "":
        return ""
    else:
        db_write(cmd)
        while line == "":
            line = db_readline()
        res += line
        while True:
            line = db_readline()
            if line != "":
                res += line
            else:
                break
        print(cmd)
        print(res)
        return res

def getDateStrings():
    ret =  []
    a = datetime.utcnow()
    a += timedelta(hours=8)
    a1 = a + timedelta(days=30)
    s = str(a.year) + "年" + str(a.month) + "月" + str(a.day) + "日"
    s1 = str(a1.year) + "年" + str(a1.month) + "月" + str(a1.day) + "日"
    ret.append(s)
    ret.append(s1)
    return ret

def getDateStringsEnglish():
    ret =  []
    a = datetime.utcnow()
    a += timedelta(hours=8)
    a1 = a + timedelta(days=30)
    s = str(a.year) + "-" + str(a.month) + "-" + str(a.day)
    s1 = str(a1.year) + "-" + str(a1.month) + "-" + str(a1.day)
    ret.append(s)
    ret.append(s1)
    return ret

def getAllorder(userid, catalog):
    currentDate = datetime.utcnow() + timedelta(hours=8)
    ret = []
    for i in range(1,32):
        syear = str(currentDate.year)
        smonth = str(currentDate.month)
        sday = str(currentDate.day)
        if len(smonth) == 1: smonth = '0' + smonth
        if len(sday) == 1: sday = '0' + sday
        datetmp = syear + '-' + smonth + '-' + sday
        cmdtmp = ' '.join(['query_order',str(userid), datetmp, str(catalog)])
        res = db_communicate(cmdtmp)
        tmptable = re.split(r'\n', res)
        if tmptable[len(tmptable) - 1] == "": tmptable.pop()
        if tmptable[0] == "0":
            currentDate += timedelta(days = 1)
            continue
        print(tmptable)
        if len(ret) > 0 :
            ret[0]  = int(ret[0]) + int(tmptable[0])
            for j in range(1, len(tmptable)):
                ret.append(tmptable[j])
        else:
            ret  = tmptable
        currentDate += timedelta(days = 1)
    print(ret)
    for i in range(1, len(ret)):
        ret[i] = re.split(r' ', ret[i])
    if len(ret) == 0:
        ret.append("0")
    return ret


@app.route('/index', methods=['GET'])
@app.route('/', methods=['GET'])
def home():
    dateList = getDateStringsEnglish()
    if request.method == 'GET':
        if 'user_id' in session and 'user_name' in session and session['user_name'] != '':
            user_name = session['user_name']
        else:
            user_name = None
        return render_template('index.html', user_name=user_name, min_date = dateList[0], max_date = dateList[1])
    else: return ""

@app.route('/queryRes', methods=['GET', 'POST'])
def queryRes():
    resList = []
    retList = []
    dateList = getDateStringsEnglish()
    catalog = ""
    min_date = dateList[0]
    max_date = dateList[1]
    if request.method == 'POST':
        if request.form['ifQueryTransfer'] == "0":
            loc1 = request.form['loc1']
            loc2 = request.form['loc2']
            ddate = request.form['ddate']
            if (loc1 == "" or loc2 == "" or ddate == "") and request.form['id'] == 'queryRes':
                return json.dumps("0")
            if 'catalog' in request.form and request.form['catalog'] != "": catalog = request.form['catalog']
            else: catalog="CDGKTZ"
            if request.form['id'] == 'queryRes':
                if loc1 == "" or loc2 == "" or ddate == "":
                    return json.dumps("")
                qcmd = ' '.join(['query_ticket', loc1, loc2, ddate, catalog])
                qstring = db_communicate(qcmd)
                resList = re.split(r'\n', qstring)
                for item in resList:
                    retList.append(re.split(r'[ ]', item))
                restmp = retList[1 : len(retList) - 1]
                restmp = sorted(restmp, key = lambda item : item[3].lower())
                for i in range(1, len(resList) - 1):
                    retList[i] = restmp[i - 1]
                retList.pop()
                qRes = json.dumps(retList)
                return qRes
            else:
                if 'user_id' in session and 'user_name' in session and session['user_name'] != None and session['user_name'] != "" and session['user_id'] != None and session['user_id'] != "":
                    user_name = session['user_name']
                    user_id = session['user_id']
                else:
                    user_id = None
                    user_name = None
                return render_template('queryRes.html', loc1 = loc1, loc2 = loc2, ddate = ddate, postFrom = request.form['id'], catalog="CDGKTZ", user_id = user_id, user_name = user_name, min_date = min_date, max_date = max_date)
        else:
             if request.form['ifQueryTransfer'] == "1":
                 loc1 = request.form['loc1']
                 loc2 = request.form['loc2']
                 ddate = request.form['ddate']
                 if loc1 == "" or loc2 == "" or ddate == "":
                     return json.dumps("0")
                 if 'catalog' in request.form and request.form['catalog'] != "": catalog = request.form['catalog']
                 else: catalog="CDGKTZ"
                 qtcmd = ' '.join(['query_transfer', loc1, loc2, ddate, catalog])
                 res = db_communicate(qtcmd)
                 if len(res) <= 3: return json.dumps("0")
                 else:
                     tmpList = re.split(r'\n', res)
                     retList = []
                     retList.append("2")
                     tmpList.pop()
                     for item in tmpList:
                         itmpList = re.split(r' ', item)
                         itmpList.pop()
                         itmpList.pop(7)
                         retList.append(itmpList)
                     return json.dumps(retList)
             else: return json.dumps("0")
    else:
        if 'user_id' in session and 'user_name' in session and session['user_name'] != '' and session['user_id'] != '':
            user_name = session['user_name']
            user_id = session['user_id']
        else:
            user_name = None
            user_id = None
        return render_template('queryRes.html', user_name = user_name, user_id = user_id, catalog="", min_date = min_date, max_date = max_date)

@app.route('/signin', methods=['POST', 'GET'])
def signin():
    if request.method == 'POST':
        userid = request.form['userid']
        password = request.form['password']
        if userid == "" or password == "": return json.dumps("0")
        reply = db_communicate(' '.join(['login', userid, password]))
        if reply[0] == "0":
            return json.dumps("0")
        else:
            session['user_id'] = userid
            reply = db_communicate(' '.join(['query_profile', userid]))
            if reply[0] == '0':
                return json.dumps("0")
            else:
                session['user_name'] = reply.split(' ')[0]
                return json.dumps("1")
    else:
        if 'user_id' in session and 'user_name' in session and session['user_name'] != None and session['user_name'] != "" and session['user_id'] != None and session['user_id'] != "":
            return redirect(url_for('userZone'))
        else:
            return render_template('signin.html')

@app.route('/signup', methods=['POST', 'GET'])
def signup():
    if request.method == 'POST':
        name = request.form['name']
        email = request.form['email']
        phone = request.form['phone']
        password = request.form['password']
        repassword = request.form['repassword']
        if password != repassword:
            return json.dumps("0")
        reply = db_communicate(' '.join(['register', name, password, email, phone]))
        if int(reply) < 2018:
            return render_template('signup.html',register_error_info="error")
        else:
            userid = int(reply);
            return render_template('signup.html',register_success_info="success", user_id = str(userid))
    else:
        if 'user_id' in session and 'user_name' in session and session['user_name'] != None and session['user_name'] != "" and session['user_id'] != None and session['user_id'] != "":
            user_name = session['user_name']
            user_id = session['user_id']
            return redirect(url_for('userZone'))
        else:
            user_name = None
            user_id = None
            return render_template('signup.html')

@app.route('/signout', methods=['GET'])
def signout():
    if 'user_id' in session:
        session.pop('user_id', None)
    if 'user_name' in session:
        session.pop('user_name', None)
    return redirect(url_for('home'))

@app.route('/orderTic', methods=['POST', 'GET'])
def orderTic():
    if request.method == 'POST':
        if 'user_id' in session and 'user_name' in session and session['user_name'] != '':
            user_name = session['user_name']
            user_id = session['user_id']
        else:
            user_name = None
            user_id = None
        if user_id != "":
            if request.form['form-name'] == "sorder":
                order_trainid = request.form['order-train-id']
                order_loc1 = request.form['order-loc1']
                order_loc2 = request.form['order-loc2']
                order_date = request.form['order-date']
                order_kind = request.form['order-kind']
                order_time1 = request.form['order-time1']
                order_time2 = request.form['order-time2']
                order_price = request.form['order-price']
                order_left = request.form['order-left']
                return render_template('orderTic.html',user_name = user_name, train_id = order_trainid, tic_loc1 = order_loc1, tic_loc2 = order_loc2, tic_date = order_date, tic_type = order_kind, tic_price = order_price, tic_left = order_left, tic_time1 = order_time1, tic_time2 = order_time2)
            else:
                if request.form['form-name'] == "corder":
                    corder_trainid = request.form['corder-train-id']
                    corder_type = request.form['corder-type']
                    corder_loc1 = request.form['corder-loc1']
                    corder_loc2 = request.form['corder-loc2']
                    corder_date = request.form['corder-date']
                    corder_num = request.form['corder-num']
                    btcmd = ' '.join(['buy_ticket', session['user_id'], corder_num, corder_trainid, corder_loc1, corder_loc2, corder_date, corder_type])
                    return json.dumps(db_communicate(btcmd))
                else:
                    return json.dumps("0")
        else:
            err_info = "未登录"
            return render_template('orderTic.html', err_info = err_info)
    else:
        err_info = "禁止访问"
        return render_template('orderTic.html', err_info = err_info)

@app.route('/refundTic', methods=['GET', 'POST'])
def refundTic():
    if request.method == 'POST':
        if 'user_id' in session and 'user_name' in session and session['user_name'] != None and session['user_name'] != "" and session['user_id'] != None and session['user_id'] != "":
            if 'form-name' in request.form:
                    if request.form['form-name'] == "refundForm":
                        train_id = request.form['refundTrainId']
                        tic_type = request.form['refundTicType']
                        tic_date = request.form['refundDate']
                        tic_loc1 = request.form['refundLoc1']
                        tic_loc2 = request.form['refundLoc2']
                        tic_num = request.form['refundNum']
                        tic_price = request.form['refundTotPrice']
                        return render_template('refundTic.html',user_name = session['user_name'], user_id = session['user_id'], train_id = train_id, tic_type = tic_type, tic_date = tic_date, tic_loc1 = tic_loc1, tic_loc2 = tic_loc2, tic_num = tic_num, tic_price = tic_price)
                    else:
                        if request.form['form-name'] == 'confirmRefundForm':
                            train_id = request.form['refund-train-id']
                            tic_type = request.form['refund-type']
                            tic_date = request.form['refund-date']
                            tic_loc1 = request.form['refund-loc1']
                            tic_loc2 = request.form['refund-loc2']
                            tic_num = request.form['refund-num']
                            return json.dumps(db_communicate(' '.join(['refund_ticket', session['user_id'], tic_num,train_id, tic_loc1, tic_loc2, tic_date, tic_type])))

            else:
                  return render_template('refundTic.html', err_info="无效的参数")
        else: return render_template('refundTic.html', err_info="登录状态错误")
    else:
        return render_template('refundTic.html', err_info = "禁止访问")



@app.route('/userZone', methods=['GET', 'POST'])
def userZone():
    if request.method == 'GET':
        timeTuple = getDateStrings()
        if 'user_id' in session and 'user_name' in session and session['user_name'] != '':
            user_name = session['user_name']
            user_id = session['user_id']
        else:
            user_name = None
            user_id = None
        retStatus = request.args.get('status')
        if retStatus == "" or retStatus == " " or retStatus == None: return render_template('userZone.html',  user_name = user_name, user_id = str(user_id) , current_time = timeTuple[0], final_time = timeTuple[1], status = "welcome")
        else: return render_template('userZone.html',  user_name = user_name, user_id = str(user_id) , current_time = timeTuple[0], final_time = timeTuple[1], status = retStatus)
    if request.method == 'POST':
        if 'user_id' in session and 'user_name' in session and session['user_name'] != '':
            user_name = session['user_name']
            user_id = session['user_id']
            return json.dumps(getAllorder(user_id, 'CDGKTZO'))
        else:
            return json.dumps("0")
    else: return ""

@app.route('/userOperator', methods=['POST'])
def userOperator():
    if request.method == 'POST' and 'user_id' in session and 'user_name' in session and session['user_name'] != None and session['user_name'] != "" and session['user_id'] != None and session['user_id'] != "":
        if 'data' in request.form:
            reqData = json.loads(request.form.get('data'))
            rqn  = reqData['requestName']
            if rqn == 'query_profile':
                uid = reqData['userId']
                print(rqn + ' ' + uid)
                rs = db_communicate(' '.join(['query_profile', uid]))
                ret = re.split(r' ', rs)
                return json.dumps(ret)
        else:
            if 'userNameText' in request.form and 'user_id' in session and session['user_id'] != '' and session['user_id'] != None:
                tmpUserName = request.form['userNameText']
                tmpUserEmail = request.form['userEmailText']
                tmpUserPhone = request.form['userPhoneText']
                tmpUserPassword = request.form['userPasswordText']
                if int(db_communicate(' '.join(['login', session['user_id'], tmpUserPassword]))) == 0: return json.dumps("0")
                else:
                    mdcmd = ' '.join(['modify_profile', session['user_id'], tmpUserName, tmpUserPassword, tmpUserEmail, tmpUserPhone])
                    ret = db_communicate(mdcmd)
                    if int(ret) == 1:
                        session['user_name'] = tmpUserName
                        return json.dumps(ret)
                    else:
                        return json.dumps("0")
            if 'userOldPasswordText' in request.form:
                 oldpwd = request.form['userOldPasswordText']
                 newpwd = request.form['userNewPasswordText']
                 newrpwd = request.form['userNewRepasswordText']
                 if newpwd != newrpwd: return json.dumps("0")
                 if int(db_communicate(' '.join(['login', session['user_id'], oldpwd]))) == 0: return json.dumps("0")
                 else:
                      usrInfo = re.split(r' ', db_communicate(' '.join(['query_profile', session['user_id']])))
                      mdcmd = ' '.join(['modify_profile', session['user_id'], usrInfo[0] , newrpwd, usrInfo[1], usrInfo[2]])
                      if int(db_communicate(mdcmd)) == 1: return json.dumps("1")
                      else: json.dumps("0")

    else: return json.dumps("0")


@app.route('/debugger', methods=['GET', 'POST'])
def debugger():
    if request.method == 'POST':
        order = request.form['order']
        if len(order.strip('\n')) == 0:
            return render_template("debugger.html")
        res = ""
        if order != "":
            res = db_communicate(order)
        return render_template("debugger.html")
    else:
        return render_template("debugger.html")

@app.route('/hzfengsy', methods=['GET'])
def hzfengsy():
    return render_template('hzfengsy.html')


if __name__ == '__main__':
    app.wsgi_app = ProxyFix(app.wsgi_app)
    app.run(host = '0.0.0.0', port = 80, debug=True)

