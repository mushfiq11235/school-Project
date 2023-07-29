import {useState, useEffect} from "react";

const Comments = ({id}) => {
    const [commentList, setCommentList]= useState([]);
    const axios= require("axios").default;

    useEffect(() => {
        
        axios.get("http://localhost:7000/api/get_recipe_review", {
            params: {
                id: Number(id)
            }
        }).then((res) => {
           setCommentList(res.data); 
        }).catch((err) => {

        });
        
    }, []);
    
    return (
        <>
            {
                commentList.map((item) => {
                    return (
                        <div key= {item.time}>
                            <p> {item.content} </p> 
                        </div>
                    );
                })
            }
        </>
    );
}

export default Comments;