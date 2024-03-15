import os
from skopt import forest_minimize
from skopt.space import Real
import glob
from log_util import get_log
from multiprocessing import Pool
from tqdm import tqdm

log, file_path = get_log("train")

total_run = 10000


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
    # print(train(paths[0], params))

    with Pool() as p:
        score_list = p.starmap(train, [(path, params) for path in paths])
    score_sum = sum(score_list)
    # score_list = []
    # for path in paths:
    #     score_list.append(train(path, params))
    # score_sum = sum(score_list)

    # for path in tqdm(paths):
    #     cmd = f'./PreliminaryJudge -m {path} "./main {params}" -l NONE'
    #     # 执行命令并获取输出
    #     result = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    #     output = result.stdout.decode('utf-8')
    #     # 解析JSON输出以获取得分
    #     try:
    #         output_json = json.loads(output)
    #         score = output_json.get('score', 0)  # 默认得分为0，如果解析失败或键不存在
    #     except json.JSONDecodeError:
    #         score = 0  # 如果输出不是有效的JSON，设置得分为0
    #     log.info(f"Map: {path}, Score: {score}")
    #     score_sum += score
    avg_score = score_sum / len(score_list)
    log.info(f"params: {params}")
    log.info(f"Score list: {score_list}")
    log.info(f"Total score: {score_sum}")
    log.info(f"Average score: {avg_score}")
    return -avg_score


def print_status(res):
    log.info(f"After call {len(res.func_vals)}: Best score: {-res.fun}")
    log.info(f"With parameters: {res.x}")
    log.info("====================================\n")


if __name__ == "__main__":
    param_space = [
        Real(0, 1.0, name='w_good_val'),
        Real(0, 1.0, name='w_good_dis'),
        Real(0, 1.0, name='w_good_disappear'),
        Real(0, 1.0, name='w_boat_speed'),
        Real(0, 1.0, name='w_boat_size'),
        Real(0, 1.0, name='w_boat_transport'),
        Real(0, 1.0, name='w_berth_vis'),
        Real(0, 1.0, name='w_berth_fill'),
        Real(0, 1.0, name='w_berth_dis'),
        Real(0, 1.0, name='w_berth_near'),
        Real(0, 1.0, name='w_berth_arrive'),
    ]

    # 使用贝叶斯优化
    result = forest_minimize(run_program, param_space, n_calls=total_run, callback=[print_status], random_state=0,
                             verbose=True)

    # 输出最优参数及其得分
    log.info("最优参数：", result.x)
    log.info("最佳得分：", -result.fun)
