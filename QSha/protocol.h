#ifndef PROTOCOL_H
#define PROTOCOL_H

enum Protocol{
    s2s_robot_ai = 0,
    s2s_send_role,

//s2c
    s2c_start,
    s2c_send_welcome,
    s2c_send_talking,
    s2c_end,
//c2s
    c2s_start,
    c2s_request_login,
    c2s_request_talking,

    c2s_request_use_skill,
    c2s_request_use_card,

    c2s_request_shunshouqianyang,
    c2s_request_guohechaiqiao,
    c2s_request_wugufengdeng,

    c2s_request_baguandun_ok,
    c2s_request_baguadun_cancel,

    c2s_request_sha_cancel,
    c2s_request_shan_cancel,
    c2s_request_tao_cancel,
    c2s_request_wuxiekeji_cancel,

    c2s_request_cixiongshugu_ok,
    c2s_request_cixiongshugu_cancel,
    c2s_request_cixiongshugu_drop,
    c2s_request_cixiongshugu_catch,

    c2s_request_qinlongyeyuedao_ok,
    c2s_request_qinlongyeyuedao_cancel,

    c2s_request_guanshifu_ok,
    c2s_request_guanshifu_cancel,
    c2s_end,
};

#endif // PROTOCOL_H
