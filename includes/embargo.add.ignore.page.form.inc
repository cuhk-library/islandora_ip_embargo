<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Form for add child object batchly.
 *
 * @param array $form
 *   Associative array with Drupal Form API elements.
 *
 */
function islandora_ip_embargo_add_ignore_page_form($form, &$form_state) {
  
  /*$form["datastream"] = array(
    '#size' => '17',
    '#type' => 'textfield',
    '#title' => t('Ignore Datastream'),
    '#required' => TRUE,
    '#description' => t(''),
  );*/
  $form["url_alias"] = array(
    '#size' => '17',
    '#type' => 'textarea',
    '#title' => t('URL Alias'),
    '#required' => TRUE,
    '#description' => t('Enter url alias need to exclude in ip embargo, please enter the url without language perfix e.g collection/drama (Not en/collection/drama'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

function islandora_ip_embargo_add_ignore_page_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
  $form_state['rebuild'] = TRUE;
  if(!empty($form_state['values']['url_alias']) || isset($form_state['values']['url_alias'])){
    $records = trim($form_state['values']['url_alias']);
    $recordsArray = explode("\n", $records);
    $recordsArray = array_filter($recordsArray, 'trim'); 
    foreach ($recordsArray as $line) {
      if(isset($line) && !empty($line)){
        $path = drupal_get_normal_path($line);
        if(!isset($path) || empty($path)){
            form_set_error('islandora_ip_embargo_add_ignore_page', t('Path '.$line.' is not valid'));
        }else{
            
            
            $pathArray = array();
            $base_node = node_load(str_replace('node/','',$path));
            $translations = translation_node_get_translations($base_node->tnid);
            foreach($translations as $translation){
                $prefix = getLanguagePrefix($translation->language);
                $pathArray['node_id'] = $translation->nid;
                $pathArray['alias'] = '/'.$prefix.'/'.$line;
                $pathExist = islandora_ip_embargo_select_ignore_page($pathArray['alias']);
                if(!isset($pathExist) || empty($pathExist)){
                    islandora_ip_embargo_add_ignore_page_list($pathArray);
                    drupal_set_message('Path '.$line.' is successfully add to exclude list');
                }else{
                    form_set_error('islandora_ip_embargo_add_ignore_page', t('Path '.$line.' exist'));
                }
            }
        }
      }
    }
  }
}

function getLanguagePrefix($lang){
    $languages = language_list('enabled');
    
    foreach($languages as $language){
        foreach($language as $item){
            if($item->language == $lang){
                return $item->prefix;
            }
        }
    }
    return False;
}
