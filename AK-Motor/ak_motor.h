/**
 * @file    ak_motor.h
 * @author  Deadline039
 * @brief   AK电机驱动代码
 * @version 1.0
 * @date    2023-11-27
 */

#ifndef __AK80_H
#define __AK80_H

#include "can.h"

/**
 * @brief 型号定义, 不同型号对于不同的参数
 */
typedef enum {
    AK10_9 = 0U,
    AK60_6,
    AK70_10,
    AK80_6,
    AK80_9,
    AK80_80_64,
    AK80_8
} ak_motor_model_t;

/**
 * @brief 电机错误信息
 */
typedef enum {
    AK_ERROR_NO_FAULT = 0U,    /*!< 无故障 */
    AK_ERROR_OVER_TEMPERATURE, /*!< 电机过温 */
    AK_ERROR_OVER_CURRENT,     /*!< 电机过流 */
    AK_ERROR_OVER_VOLTAGE,     /*!< 电机过压 */
    AK_ERROR_UNDER_VOLTAGE,    /*!< 电机欠压 */
    AK_ERROR_ENCODER_FAULT,    /*!< 编码器故障 */
    AK_ERROR_MOS_TEMPERATURE,  /*!< MOS过温 */
    AK_ERROR_ROTOR_LOCK        /*!< 电机堵转 */
} ak_motor_error_t;

/**
 * @brief AK电机句柄
 */
typedef struct {
    can_select_t can_select; /*!< 选择CAN1还是CAN2 */
    uint32_t controller_id;  /*!< CAN ID */

    ak_motor_model_t motor_model; /*!< 电机型号 */
    float motor_pos;              /*!< 电机位置 */
    float motor_spd;              /*!< 电机速度 */
    float motor_cur_troq;         /*!< 电机电流, 运控模式为扭矩 */
    int8_t motor_temperature;     /*!< 电机温度 */
    ak_motor_error_t error_code;  /*!< 电机错误码 */
} ak_motor_handle_t;

/**
 * @brief 设置原点模式
 */
typedef enum {
    SET_ORIGIN_TEMPORARY = 0U, /*!< 设置临时原点(断电消除) */
    SET_ORIGIN_PERMANENT,      /*!< 设置永久零点(参数自动保存) */
    SET_ORIGIN_RESET_DEFAULT   /*!< 恢复默认零点(参数自动保存) */
} ak_origin_mode_t;

void ak_motor_init(ak_motor_handle_t *motor, uint32_t id,
                   ak_motor_model_t model, can_select_t can_select);
void ak_motor_deinit(ak_motor_handle_t *motor);

/* 伺服模式 */
void ak_servo_set_duty(ak_motor_handle_t *motor, float duty);
void ak_servo_set_current(ak_motor_handle_t *motor, float current);
void ak_servo_set_cb(ak_motor_handle_t *motor, float current);
void ak_servo_set_rpm(ak_motor_handle_t *motor, float rpm);
void ak_servo_set_pos(ak_motor_handle_t *motor, float pos);
void ak_servo_set_origin(ak_motor_handle_t *motor,
                         ak_origin_mode_t set_origin_mode);
void ak_servo_set_pos_spd(ak_motor_handle_t *motor, float pos, float spd,
                          float rpa);

/* 运控模式 */
void ak_mit_enter_motor(ak_motor_handle_t *motor);
void ak_mit_set_origin(ak_motor_handle_t *motor);
void ak_mit_send_data(ak_motor_handle_t *motor, float pos, float spd, float kp,
                      float kd, float torque);
void ak_mit_exit_motor(ak_motor_handle_t *motor);

#endif /* __AK80_H */
