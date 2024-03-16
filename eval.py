import os
import glob
from log_util import get_log
from multiprocessing import Pool

log, file_path = get_log("eval")


def train(path, params):
    # 执行命令并获取输出
    # result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # judge = "./PreliminaryJudge" + path.split(".txt")[0][-1]
    # exe = "./main" + path.split(".txt")[0][-1]
    judge = "./PreliminaryJudge"
    exe = "./main"
    cmd = f'{judge} -m {path} "{exe} {params}" -l NONE'
    result = os.popen(cmd).read()
    output = dict(eval(result))
    # {"status":"Successful","score":89424}
    if output['status'] == 'Successful':
        score = output['score']
    else:
        score = -1
    # print(f"Map: {path}, Score: {score}")
    return score


def run_program(params):
    # 解包参数
    w_good_val, w_good_dis, w_good_disappear, w_boat_speed, w_boat_size, w_boat_transport, w_berth_vis, w_berth_fill, w_berth_dis, w_berth_near, w_berth_arrive = params

    params = " ".join(map(str, [w_good_val, w_good_dis, w_good_disappear, w_boat_speed, w_boat_size, w_boat_transport,
                                w_berth_vis, w_berth_fill, w_berth_dis, w_berth_near, w_berth_arrive]))

    paths = sorted(glob.glob("./maps/*.txt"))
    with Pool() as p:
        score_list = p.starmap(train, [(path, params) for path in paths])
    score_sum = sum(score_list)
    avg_score = score_sum / len(score_list)
    log.info(f"Params: {params}")
    log.info(f"Score list: {score_list}")
    log.info(f"Total score: {score_sum}")
    log.info(f"Average score: {avg_score}")
    return -avg_score


if __name__ == "__main__":
    params = (0.0005219068103488046 0.7612874007557432 0.04765758497181795 0.37260152108095673 0.5450687184080321 0.41067724512928994 0.25073264917064636 0.9197148650811919 0.06689609151687516 2.1718084691313425e-05 0.6409714467691711)
    run_program(params)
