<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


function islandora_ip_embargo_exceptional_pages_list_form(array $form, array &$form_state){
    module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
    $form['add_exclude_page'] = array(
        '#markup' => l(t('Add exclude page'), 'admin/islandora/tools/ip_embargo/ignore_list/add'),
    );
    
    $form['filter_record'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="filter_object">',
        '#tree' => TRUE,
    );
    $form['filter_record']['text'] = array(
       '#type' => 'textarea',
       '#title' => t('Filter by url'),
    );
    $form['filter_record']['submit'] = array(
       '#type' => 'submit', 
       '#value' => t('Filter'),
       '#submit' => array('filter_record'),
    );
    $form['filter_record']['reset_record'] = array(
       '#type' => 'submit', 
       '#value' => t('Reset Record'),
       '#submit' => array('reset_record'),
    );
    $options = array();
    
    if(isset($_SESSION['islandora_ip_embargo_filter_exception_list'])){
        $records = trim($_SESSION['islandora_ip_embargo_filter_exception_list']);
        $recordsArray = explode("\n", $records);
        $recordsArray = array_filter($recordsArray, 'trim');
        $results = islandora_ip_embargo_filter_ignore_page_list($recordsArray);
    }else{
        $results = islandora_ip_embargo_select_ignore_page_list();
    }
    global $base_url;
    foreach($results as $value){
        $options[$value['id']] = array(
          'id' => trim($value['id']),
          'node_id' => trim($value['node_id']),
          'alias' => l($value['alias'],$base_url.$value['alias']),
        );
    }
    
    $header = array('id' => t('ID'),'node_id' => t('Node ID'),'alias' => t('URL Alias'));
    
    $form['embargo_object_exclude_list'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#attributes' => array(),
        '#empty' => t("There are no url exclude from embargo."),
    );
    
    $form['embargo_object_exclude_list_pager'] = array(
        '#markup' => theme('pager'),
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Delete Objects Embargo'));

    return $form;
}

function filter_record($form, &$form_state) {
  if(isset($form_state['values']['filter_record']['text'])){
    if(isset($form_state['values']['filter_record']['text']) && !empty($form_state['values']['filter_record']['text'])){
      $form_state['storage']['filter_record']['text'] = $form_state['values']['filter_record']['text'];
      $_SESSION['islandora_ip_embargo_filter_exception_list'] = $form_state['values']['filter_record']['text'];
    }
    $form_state['rebuild'] = TRUE;
  }
}

/*
 * Remove session variable
 */

function reset_record(){
    if(isset($_SESSION['islandora_ip_embargo_filter_exception_list'])){
        unset($_SESSION['islandora_ip_embargo_filter_exception_list']);
    }
}

function islandora_ip_embargo_exceptional_pages_list_form_submit(array $form, array &$form_state) {
    module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
    foreach ($form_state['values']['embargo_object_exclude_list'] as $list_identifier => $is_checked) {
      if ($is_checked) {
        islandora_ip_embargo_remove_ignore_page_list(trim($list_identifier));
      }
    }
}
